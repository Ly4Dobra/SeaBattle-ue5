// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
	CellMesh->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));

	if (MeshAsset.Succeeded())
	{
		CellMesh->SetStaticMesh(MeshAsset.Object);

	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Material/M_CellMaterial.M_CellMaterial"));
	if (MaterialAsset.Succeeded())
	{
		CellMesh->SetMaterial(0, MaterialAsset.Object);
	}

	DynMat = CellMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (DynMat)
	{
		DynMat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.0f, 0.2f, 0.8f, 1.0f));
	}

}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector(MoveSpeed * DeltaTime, 0.0f, 0.0f));

}

void ACell::MarkHit()
{
	IsHit = true;
	UE_LOG(LogTemp, Warning, TEXT("Строка: %d Колонка: %d Подбит: %d"), Row, Column, IsHit ? 1 : 0);
}
