// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

AStaircaseActor::AStaircaseActor() : NumberOfStaircases{ 10 }, Dimensions{ 2.0, 0.6, 0.34 }, StairMesh{}, HasRailings{false}, RailingMesh{}
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene");
	SetRootComponent(RootScene);
}

void AStaircaseActor::BeginPlay()
{
	Super::BeginPlay();
}

void AStaircaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStaircaseActor::OnConstruction(const FTransform& Transform) {
	UE_LOG(LogTemp, Warning, TEXT("On Construction Called"));

	for (int32 i = 0; i < StaircaseComponents.Num(); ++i) {
		if (StaircaseComponents[i]) {
			StaircaseComponents[i]->DestroyComponent();
			StaircaseComponents[i] = nullptr;
		}
	}

	for (int32 i = 0; i < NumberOfStaircases; ++i) {

		FString ComponentName = "Stair" + FString::FromInt(i);
		UStaticMeshComponent* Stair = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *ComponentName);

		if (StairMesh) {
			Stair->SetStaticMesh(StairMesh);
		}

		Stair->SetRelativeScale3D(Dimensions);
		Stair->AddLocalOffset(FVector(0, (Dimensions.Y * 100 * i), (Dimensions.Z * 125 * i)));
		Stair->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Stair->RegisterComponentWithWorld(GetWorld());

		StaircaseComponents.Add(Stair);
	}
}