// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"
#include "Kismet/KismetMathLibrary.h"

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
	float DistanceCovered{ FenceProperties.Length /2 };

	while (DistanceCovered + FenceProperties.Length <= SplineLength) {
		FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceCovered, ESplineCoordinateSpace::Local);
		StartLocation.Z += FenceProperties.Height / 2;

		UStaticMeshComponent* RailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		RailingStaticMeshComponent->RegisterComponent();
		RailingStaticMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		if (RailingStaticMesh) {
			RailingStaticMeshComponent->SetStaticMesh(RailingStaticMesh);
		}
		
		RailingStaticMeshComponent->SetRelativeLocation(StartLocation);
		RailingStaticMeshComponent->SetWorldScale3D(FVector{ FenceProperties.Length / 10, FenceProperties.Width / 10, FenceProperties.Height / 100});

	
		RailingStaticComponents.Add(RailingStaticMeshComponent);

		DistanceCovered += TotalSpacing;
	}
	GenerateStaticHorizontalBeam();
}

void AFenceMeshActor::DestroyStaticFence() {
	for (int32 i = 0; i < RailingStaticComponents.Num(); ++i) {
		if (RailingStaticComponents[i]) {
			RailingStaticComponents[i]->DestroyComponent();
			RailingStaticComponents[i] = nullptr;
		}
	}

	RailingStaticComponents.Empty();

	DestroyStaticHorizontalBeam();
}

void AFenceMeshActor::GenerateProceduralFence() {

	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();
	float SplineLength = SplineComponent->GetDistanceAlongSplineAtSplinePoint(SplinePointsCount - 1);

	float TotalSpacing = FenceProperties.Length + FenceProperties.Spacing;
	float DistanceCovered{ FenceProperties.Length / 2 };

	//int32 RailingsCount = FMath::FloorToInt(SplineLength/ TotalSpacing);

	while (DistanceCovered + FenceProperties.Length <= SplineLength) {
		FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceCovered, ESplineCoordinateSpace::World);
		StartLocation.Z += FenceProperties.Height / 2;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVerticalRailActor* SpawnedVerticalRailActor = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailActorClass, StartLocation, FRotator::ZeroRotator, SpawnParams);
		SpawnedVerticalRailActor->CreateVerticalRailActor(FVector{FenceProperties.Length, FenceProperties.Width, FenceProperties.Height});

		DistanceCovered += TotalSpacing;
	}
	GenerateProceduralHorizontalBeam();
}

void AFenceMeshActor::GenerateStaticHorizontalBeam() {
	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();

	for (int32 i = 0; i < SplinePointsCount - 1; ++i) {
		float Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i+1) - SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);

		//Up
		UStaticMeshComponent* HorizontalBeamStaticMeshComponentUp = NewObject<UStaticMeshComponent>(this);
		HorizontalBeamStaticMeshComponentUp->RegisterComponent();
		HorizontalBeamStaticMeshComponentUp->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		HorizontalBeamStaticComponents.Add(HorizontalBeamStaticMeshComponentUp);

		if (HorizontalBeamStaticMesh) {
			HorizontalBeamStaticMeshComponentUp->SetStaticMesh(HorizontalBeamStaticMesh);
		}
		FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		FVector ForwardDirection = UKismetMathLibrary::GetDirectionUnitVector(StartLocation, EndLocation);
		FVector UpDirection = UKismetMathLibrary::GetUpVector(GetActorRotation());
		FVector RightDirection = UKismetMathLibrary::RotateAngleAxis(ForwardDirection, 90, UpDirection);

		FRotator BeamRotation = UKismetMathLibrary::MakeRotationFromAxes(ForwardDirection, RightDirection, UpDirection);

		StartLocation += (ForwardDirection * (Distance / 2));
		StartLocation += (-RightDirection * ((FenceProperties.Width + 3) / 2));
		StartLocation += (UpDirection * (3 * FenceProperties.Height / 4));

		HorizontalBeamStaticMeshComponentUp->SetWorldRotation(BeamRotation);
		HorizontalBeamStaticMeshComponentUp->SetRelativeLocation(StartLocation);
		HorizontalBeamStaticMeshComponentUp->SetWorldScale3D(FVector{ (Distance + 16) / 100, FenceProperties.Width / 10, FenceProperties.Height/100 });
	
		//Down
		UStaticMeshComponent* HorizontalBeamStaticMeshComponentDown = NewObject<UStaticMeshComponent>(this);
		HorizontalBeamStaticMeshComponentDown->RegisterComponent();
		HorizontalBeamStaticMeshComponentDown->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		HorizontalBeamStaticComponents.Add(HorizontalBeamStaticMeshComponentDown);

		if (HorizontalBeamStaticMesh) {
			HorizontalBeamStaticMeshComponentDown->SetStaticMesh(HorizontalBeamStaticMesh);
		}
		StartLocation += (-UpDirection * (3 * FenceProperties.Height / 4));
		StartLocation += (UpDirection * (FenceProperties.Height / 3));

		HorizontalBeamStaticMeshComponentDown->SetWorldRotation(BeamRotation);
		HorizontalBeamStaticMeshComponentDown->SetRelativeLocation(StartLocation);
		HorizontalBeamStaticMeshComponentDown->SetWorldScale3D(FVector{ (Distance + 16) / 100, FenceProperties.Width / 10, FenceProperties.Height/ 100 });
	}
}

void AFenceMeshActor::GenerateProceduralHorizontalBeam() {
	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();

	for (int32 i = 0; i < SplinePointsCount - 1; ++i) {
		float Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i + 1) - SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);

		FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

		FVector ForwardDirection = UKismetMathLibrary::GetDirectionUnitVector(StartLocation, EndLocation);
		FVector UpDirection = UKismetMathLibrary::GetUpVector(GetActorRotation());
		FVector RightDirection = UKismetMathLibrary::RotateAngleAxis(ForwardDirection, 90, UpDirection);

		FRotator BeamRotation = UKismetMathLibrary::MakeRotationFromAxes(ForwardDirection, RightDirection, UpDirection);
		// Up
		StartLocation += (ForwardDirection * (Distance / 2));
		StartLocation += (-RightDirection * ((FenceProperties.Width + 3) / 2));
		StartLocation += (UpDirection * (3 * FenceProperties.Height / 4));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVerticalRailActor* HorizontalBeamActorUp = GetWorld()->SpawnActor<AVerticalRailActor>(StartLocation, BeamRotation, SpawnParams);
		HorizontalBeamActorUp->GenerateCube(0, FVector{Distance + 16, (3*FenceProperties.Width)/10 , (10*FenceProperties.Height) / 100});
		
		// Down
		StartLocation += (-UpDirection * (3 * FenceProperties.Height / 4));
		StartLocation += (UpDirection * (FenceProperties.Height / 3));

		AVerticalRailActor* HorizontalBeamActorDown = GetWorld()->SpawnActor<AVerticalRailActor>(StartLocation, BeamRotation, SpawnParams);
		HorizontalBeamActorDown->GenerateCube(0, FVector{ Distance + 16, (3 * FenceProperties.Width) / 10 , (10 * FenceProperties.Height) / 100 });
	}
}

void AFenceMeshActor::DestroyStaticHorizontalBeam() {
	for (int32 i = 0; i < HorizontalBeamStaticComponents.Num(); ++i) {
		if (HorizontalBeamStaticComponents[i]) {
			HorizontalBeamStaticComponents[i]->DestroyComponent();
			HorizontalBeamStaticComponents[i] = nullptr;
		}
	}

	HorizontalBeamStaticComponents.Empty();
}
