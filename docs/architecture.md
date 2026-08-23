# Архитектура SeaBattle — схемы классов (UML)

> Версия: 1.0 · Дата: авг 2026 · Живой документ: обновлять при изменении кода
> Схемы: **Mermaid classDiagram** — GitHub рендерит их прямо в браузере, без плагинов.
> Как смотреть: открыть этот файл на GitHub → увидеть диаграммы. Или локально: расширение
> «Markdown Preview Mermaid Support» в VS Code.

---

## 1. Два слоя: Core (чистый C++) и UE (движок)

```
┌────────────────────────────────────────────────────┐
│ UI (UMG): меню, лобби, поле, результат             │  ← Blueprint 20%
├────────────────────────────────────────────────────┤
│ UE-слой (презентация + сеть)                      │  ← C++ 80%
│ GameMode / GameState / PlayerController / Pawn     │
│ BoardActor / ShipActor / UMG-виджеты               │
├────────────────────────────────────────────────────┤
│ CORE (чистый C++, БЕЗ UE-заголовков)               │  ← gtest
│ поле, корабли, правила, ИИ                         │
└────────────────────────────────────────────────────┘
```

**Главное правило:** Core не знает о UE. Стрелки зависимостей идут ТОЛЬКО сверху вниз
(UE → Core). Это даёт: gtest без редактора, чистые интерфейсы, быстрый CI.

---

## 2. Core — игровая логика (без UE, покрывается gtest)

```mermaid
classDiagram
    direction LR

    class FCellCoord {
        +int Row
        +int Col
        +bool IsValid(int BoardSize) const
        +bool operator==(const FCellCoord&) const
    }

    class FShip {
        +int Size
        +EOrientation Orientation
        +TArray~bool~ DecksHit
        +bool IsSunk() const
        +void HitDeck(int Index)
    }

    class FBattleField {
        -int Size
        -TArray~FShip~ Ships
        -TArray~ECellState~ Cells
        +bool CanPlace(const FShip&, FCellCoord Start) const
        +bool PlaceShip(const FShip&, FCellCoord Start)
        +FShotResult Shoot(FCellCoord Cell)
        +bool IsFleetDestroyed() const
        +TArray~FShip~ GetShips() const
    }

    class FShotResult {
        +EShotResult Result
        +int SunkShipIndex
    }

    class FShipPlacer {
        +TArray~FShip~ GenerateRandomFleet(int BoardSize)
    }

    class IShootStrategy {
        <<interface>>
        +FCellCoord NextShot(const FBattleField& EnemyField)
    }

    class FRandomAI {
        +FCellCoord NextShot(const FBattleField& EnemyField)
    }

    class FHunterAI {
        -FCellCoord LastHit
        -TArray~FCellCoord~ Targets
        +FCellCoord NextShot(const FBattleField& EnemyField)
    }

    class FProbabilityAI {
        -TArray~int~ ProbabilityMap
        +FCellCoord NextShot(const FBattleField& EnemyField)
        -void RebuildMap(const FBattleField&)
    }

    FBattleField "1" --> "*" FShip : владеет
    FBattleField --> FShotResult : возвращает
    FBattleField ..> IShootStrategy : использует (для ИИ)
    FShipPlacer --> FShip : создаёт
    IShootStrategy <|-- FRandomAI
    IShootStrategy <|-- FHunterAI
    IShootStrategy <|-- FProbabilityAI
```

### Ключевые решения Core
- **`FCellCoord`** — маленькая value-структура (ряд + колонка). Никаких UE-векторов в Core.
- **`FBattleField`** — единственная точка входа в логику: расстановка, выстрел, победа.
- **`IShootStrategy`** — интерфейс ИИ (чистая виртуальная). Три реализации = три уровня сложности.
  Подмена стратегии без изменения `FBattleField` — это **полиморфизм** (главы 11–12 Праты).
- Никаких `#include "Engine/..."` — только стандартная библиотека C++.

---

## 3. UE-слой — презентация и сеть

```mermaid
classDiagram
    direction LR

    class AGameModeBase
    class AGameStateBase
    class APlayerController
    class APawn
    class UUserWidget

    class ASeaBattleGameMode {
        +EMatchPhase MatchPhase
        +ETurn CurrentTurn
        +void StartMatch()
        +void ProcessShot(FCellCoord Cell)
        +void OnPlayerReady()
    }

    class ASeaBattleGameState {
        +FBattleField FieldA
        +FBattleField FieldB
        +ETurn CurrentTurn
        +EMatchPhase MatchPhase
    }

    class ASeaBattlePlayerController {
        +void RequestShot(FCellCoord Cell)
        -void Server_Shoot(FCellCoord Cell)  // RPC
        +void Client_OnShotResult(FShotResult)
    }

    class ASeaBattlePawn {
        +float CameraHeight
    }

    class UBattleFieldWidget {
        +void OnCellClicked(FCellCoord Cell)
        +void UpdateBoard(const FBattleField& Field)
        +void SetTurn(ETurn Turn)
    }

    AGameModeBase <|-- ASeaBattleGameMode
    AGameStateBase <|-- ASeaBattleGameState
    APlayerController <|-- ASeaBattlePlayerController
    APawn <|-- ASeaBattlePawn
    UUserWidget <|-- UBattleFieldWidget

    ASeaBattleGameMode --> ASeaBattleGameState : владеет/реплицирует
    ASeaBattleGameMode --> FBattleField : держит логику (Core)
    ASeaBattlePlayerController --> ASeaBattleGameMode : RPC-вызов
    ASeaBattlePlayerController --> UBattleFieldWidget : показывает
    ASeaBattleGameState --> FBattleField : реплицируемое состояние
```

### Как это работает в сети
1. Клиент кликает по клетке → `UBattleFieldWidget::OnCellClicked`.
2. `PlayerController::RequestShot` → RPC на сервер `Server_Shoot`.
3. Сервер: `GameMode::ProcessShot` → вызывает **Core** (`FBattleField::Shoot`) → результат.
4. Сервер обновляет `GameState` → репликация всем → виджеты обновляются.
5. Очередь хода и валидация — на сервере (клиент не может сжульничать).

---

## 4. Как реализовывать в коде (правила)

### 4.1 Заголовок = контракт, .cpp = реализация
```cpp
// Core/Battlefield.h — только объявление (контракт)
#pragma once

struct FCellCoord { ... };
class FBattleField { ... };  // объявления методов

// Core/Battlefield.cpp — реализация
#include "Battlefield.h"
// ... тела методов
```

### 4.2 Интерфейс = чистая виртуальная (для ИИ)
```cpp
// Core/AI/IShootStrategy.h
class IShootStrategy {
public:
    virtual ~IShootStrategy() = default;
    virtual FCellCoord NextShot(const FBattleField& EnemyField) = 0; // = 0 → чистая виртуальная
};
```

### 4.3 UE-классы наследуют движковые (GameMode, Controller...)
```cpp
UCLASS()
class SEABATTLE_API ASeaBattleGameMode : public AGameModeBase
{
    GENERATED_BODY()
    // ...
};
```

### 4.4 Держи схему в синхроне с кодом
- Изменил класс → обнови mermaid-блок в этом файле (1–2 минуты).
- Схема — «карта», а не «закон»: если код ушёл вперёд, схема обновляется за ним.

---

## 5. Альтернативы хранения схем (если захочешь развить)

| Инструмент | Где смотреть | Плюсы | Минусы |
|---|---|---|---|
| **Mermaid (этот файл)** ⭐ | GitHub, VS Code | Рендер на GitHub, живёт в git, diff виден | Нет интерактива |
| PlantUML | Нужен рендер (плагин/CI) | Мощнее, много диаграмм | GitHub не рендерит нативно |
| draw.io (.drawio) | draw.io, VS Code | Интерактив, drag&drop | Бинарный XML, diff плохой |
| Doxygen | Сгенерированный HTML | Авто-доки из кода | Требует настройки, отдельный билд |

**Рекомендация:** Mermaid в `docs/` — нативный рендер GitHub, живая история в git.
Doxygen — опционально на этапе 4, когда код станет большим.
