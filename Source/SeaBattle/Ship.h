// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship.generated.h"

UCLASS()
class SEABATTLE_API AShip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Ship")
	FString ShipName = TEXT("Линкор");
	
	UPROPERTY(EditAnywhere, Category = "Ship")
	int32 ShipSize{ 4 };
	
	UPROPERTY(EditAnywhere, Category = "Ship")
	int32 ShipHP{ 4 };

	UPROPERTY(EditAnywhere, Category = "Ship")
	int32 TeamID{ 1 };

	UPROPERTY(EditAnywhere, Category = "Ship")
	bool IsDestroyed = false;

	UFUNCTION(CallInEditor, Category = "Ship")
	void Fire();

	UFUNCTION(CallInEditor, Category = "Ship")
	void TakeDamage();
	
	UFUNCTION(CallInEditor, Category = "Ship")
	void Repair();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
