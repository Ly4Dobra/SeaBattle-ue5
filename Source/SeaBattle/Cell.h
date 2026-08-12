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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Cell");
	int32 Row{ 0 };

	UPROPERTY(EditAnywhere, Category = "Cell");
	int32 Column{ 0 };

	UPROPERTY(EditAnywhere, Category = "Cell");
	bool IsHit = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
