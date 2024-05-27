// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline Component");
	SplineComponent->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AFenceMeshActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFenceMeshActor::GenerateStaticFence() {
	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();

	float SplineLength = SplineComponent->GetDistanceAlongSplineAtSplinePoint(SplinePointsCount-1);

	for (int32 i = 0; i < SplinePointsCount; ++i) {
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
	}

	float TotalSpacing = FenceProperties.Length + FenceProperties.Spacing;
	float DistanceCovered{0.0f};

	//int32 RailingsCount = FMath::FloorToInt(SplineLength/ TotalSpacing);

	while (DistanceCovered + FenceProperties.Length <= SplineLength) {
		FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceCovered, ESplineCoordinateSpace::Local);
		StartLocation.Z += FenceProperties.Height / 2;

		UStaticMeshComponent* RailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		if (RailingStaticMesh) {
			RailingStaticMeshComponent->SetStaticMesh(RailingStaticMesh);
		}
		
		RailingStaticMeshComponent->SetRelativeLocation(StartLocation);
		RailingStaticMeshComponent->SetWorldScale3D(FVector{ FenceProperties.Length / 10, FenceProperties.Width / 10, FenceProperties.Height / 100});
		RailingStaticMeshComponent->RegisterComponent();
		RailingStaticMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
		RailingStaticMeshes.Add(RailingStaticMeshComponent);

		DistanceCovered += TotalSpacing;
	}
}

void AFenceMeshActor::DestroyStaticFence() {
	for (int32 i = 0; i < RailingStaticMeshes.Num(); ++i) {
		if (RailingStaticMeshes[i]) {
			RailingStaticMeshes[i]->DestroyComponent();
			RailingStaticMeshes[i] = nullptr;
		}
	}

	RailingStaticMeshes.Empty();
}

void AFenceMeshActor::GenerateProceduralFence() {

	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();
	float SplineLength = SplineComponent->GetDistanceAlongSplineAtSplinePoint(SplinePointsCount - 1);

	float TotalSpacing = FenceProperties.Length + FenceProperties.Spacing;
	float DistanceCovered{ 0.0f };

	//int32 RailingsCount = FMath::FloorToInt(SplineLength/ TotalSpacing);

	while (DistanceCovered + FenceProperties.Length <= SplineLength) {
		FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceCovered, ESplineCoordinateSpace::World);
		StartLocation.Z += FenceProperties.Height / 2;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVerticalRailActor* SpawnedVerticalRailActor = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailActorClass, StartLocation, FRotator::ZeroRotator, SpawnParams);
		SpawnedVerticalRailActor->CreateVerticalRailActor(FVector{FenceProperties.Length, FenceProperties.Width, FenceProperties.Height});

		VerticalRailActors.Add(SpawnedVerticalRailActor);

		DistanceCovered += TotalSpacing;
	}
}
