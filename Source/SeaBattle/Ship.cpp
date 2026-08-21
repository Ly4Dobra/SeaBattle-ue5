// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Fab/pirate_ship/pirateship1.pirateship1"));
	if (MeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(MeshAsset.Object);
	}


}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	DefPosition = FVector(GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Корабль %s, размер %d, здоровье %d, команда %d, %s"), *ShipName, ShipSize, ShipHP, TeamID, 
		IsDestroyed ? TEXT("потоплен") : TEXT("не потоплен"));
	
	SetActorScale3D(Scale);
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaPosition = GetActorLocation().X - DefPosition.X;
	
	if (!bisRotation)
	{
		
		if ((bMoveRight && DeltaPosition > Distance) || (!bMoveRight && DeltaPosition < -Distance))
		{
			bisRotation = true;
		}


	}

	if (bisRotation)
	{
		float Rotation = TurnSpeed * DeltaTime;
		AddActorWorldRotation(FRotator(0.0f, Rotation, 0.0f));
		TotalRotation += Rotation;

		if (TotalRotation >= 180.0f)
		{
			bMoveRight = !bMoveRight;
			bisRotation = false;
			TotalRotation = 0.0f;
		}
	}
	else
	{
		AddActorWorldOffset(FVector((bMoveRight ? MoveSpeed : -MoveSpeed) * DeltaTime, 0.0f, 0.0f));
	}
}

void AShip::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Корабль %s производит выстрел!"), *ShipName);
}

void AShip::TakeDamage(int32 Damage)
{
	if (Damage == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Нет урона, нет проблем!"));
		return;
	}
	
	ShipHP -= Damage;
	if (ShipHP < 0) ShipHP = 0;
	if (ShipHP == 0) IsDestroyed = true;
	
	if (IsDestroyed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Корабль %s получил урон. Корабль потоплен!"), *ShipName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Корабль %s получил урон. Текущий уровень HP - %d"), *ShipName, ShipHP);
	}

}

void AShip::Repair()
{
	if (IsDestroyed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Корабль %s потоплен. Ремонт невозможен!"), *ShipName);
		return;
	}
	
	ShipHP = ShipSize;
	UE_LOG(LogTemp, Warning, TEXT("Корабль %s отремонтирован. Текущий уровень HP - %d"), *ShipName, ShipHP);
}

void AShip::PrintPasport()
{
	UE_LOG(LogTemp, Warning, TEXT("Имя: %s | Путь: %s"),
		*GetName(), *GetPathName());
}

void AShip::ApplyDamageFromEditor()
{
	// Вызываем вашу основную функцию, передавая значение из панели деталей
	TakeDamage(EditorDamageToApply);
}

void AShip::PrintRandomNumber()
{
	int32 RandomValue = UKismetMathLibrary::RandomInteger(100);
	UE_LOG(LogTemp, Warning, TEXT("Случайное число: %d"), RandomValue);
}

#if WITH_EDITOR
void AShip::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Проверяем, изменилось ли именно наше свойство
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AShip, TurnSpeed))
	{
		// Если ввели ноль, принудительно меняем его на безопасное значение
		if (FMath::IsNearlyZero(TurnSpeed))
		{
			TurnSpeed = 1.0f;
		}
	}
}
#endif
