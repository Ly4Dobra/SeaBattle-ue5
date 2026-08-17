// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS()
class SEABATTLE_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();

	UPROPERTY(EditAnywhere, Category = "Cell")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = "Cell")
	UStaticMeshComponent* CellMesh;

	UPROPERTY()
	class UMaterialInstanceDynamic* DynMat;

	UPROPERTY(EditAnywhere, Category = "Cell")
	float MoveSpeed{ 50.0f };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Cell")
	int32 Row{ 0 };

	UPROPERTY(EditAnywhere, Category = "Cell")
	int32 Column{ 0 };

	UPROPERTY(EditAnywhere, Category = "Cell")
	bool IsHit = false;

	UFUNCTION(CallInEditor, Category = "Cell")
	void MarkHit();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
