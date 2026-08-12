// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("Конструктор корабля отработал!"));

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

void AShip::TakeDamage()
{
	ShipHP -= 1;
	UE_LOG(LogTemp, Warning, TEXT("Корабль %s получил урон. Текущий уровень HP - %d"), *ShipName, ShipHP);
}

void AShip::Repair()
{
	ShipHP = ShipSize;
	UE_LOG(LogTemp, Warning, TEXT("Корабль %s отремонтирован. Текущий уровень HP - %d"), *ShipName, ShipHP);
}
