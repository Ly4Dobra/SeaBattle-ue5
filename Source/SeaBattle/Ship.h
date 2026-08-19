// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
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

	UPROPERTY(EditAnywhere, Category = "Ship")
	USceneComponent* Root;
	UStaticMeshComponent* ShipMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// ============ Свойства ============
	UPROPERTY(EditAnywhere, Category = "Ship")
	FString ShipName{ TEXT("Линкор") };
	
	UPROPERTY(EditAnywhere, Category = "Ship")
	int32 ShipSize{ 4 };
	
	UPROPERTY(EditAnywhere, Category = "Ship")
	int32 ShipHP{ 4 };

	UPROPERTY(EditAnywhere, Category = "Ship")
	int32 TeamID{ 1 };

	UPROPERTY(EditAnywhere, Category = "Ship")
	bool IsDestroyed{ false };

	UPROPERTY(EditAnywhere, Category = "Ship")
	int32 EditorDamageToApply{7};

	UPROPERTY(EditAnywhere, Category = "Ship")
	float MoveSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, Category = "Ship")
	bool bMoveRight{ true };

	UPROPERTY(EditAnywhere, Category = "Ship")
	float TurnSpeed{ 0.0f };

	UPROPERTY(EditAnywhere, Category = "Ship")
	float Distance{ 1000.0f };

	UPROPERTY(EditAnywhere, Category = "Ship")
	FVector Scale{ 0.5f, 0.5f, 0.5f };

	// ============ Служебные свойства ============
	UPROPERTY()
	FVector DefPosition;
	
	UPROPERTY()
	float TotalRotation{ 0.0f };

	UPROPERTY()
	bool isRotation{ false };


	// ============ Действия ============
	UFUNCTION(CallInEditor, Category = "Ship")
	void Fire();

	UFUNCTION(CallInEditor, Category = "Ship")
	void TakeDamage(int32 Damage);
	
	UFUNCTION(CallInEditor, Category = "Ship")
	void ApplyDamageFromEditor();

	UFUNCTION(CallInEditor, Category = "Ship")
	void Repair();
	
	UFUNCTION(CallInEditor, Category = "Ship")
	void PrintRandomNumber();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
