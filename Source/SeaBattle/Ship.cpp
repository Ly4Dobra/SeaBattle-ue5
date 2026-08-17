// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	UE_LOG(LogTemp, Warning, TEXT("Корабль %s, размер %d, здоровье %d, команда %d, %s"), *ShipName, ShipSize, ShipHP, TeamID, 
		IsDestroyed ? TEXT("потоплен") : TEXT("не потоплен"));
	
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
