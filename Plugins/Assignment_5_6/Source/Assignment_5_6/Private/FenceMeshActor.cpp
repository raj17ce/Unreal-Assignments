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

void AFenceMeshActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if (SourceMaterial && !DynamicMaterial) {
		DynamicMaterial = UMaterialInstanceDynamic::Create(SourceMaterial, this);
	}
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

		FRotator StartRotation = SplineComponent->GetRotationAtDistanceAlongSpline(DistanceCovered, ESplineCoordinateSpace::Local);

		UStaticMeshComponent* RailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		RailingStaticMeshComponent->RegisterComponent();
		RailingStaticMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		if (VerticalRailAssets && VerticalRailAssets->DataMap.Num() > 0) {
			if (auto RailingStaticMesh = VerticalRailAssets->DataMap[TopMeshType].StaticMesh) {
				RailingStaticMeshComponent->SetStaticMesh(RailingStaticMesh);
			}
		}

		if (DynamicMaterial) {
			float TileX = (FenceProperties.Length + FenceProperties.Width) / 20.0f;
			float TileY = FenceProperties.Height / 50.0f;
			DynamicMaterial->SetScalarParameterValue("TileX", TileX);
			DynamicMaterial->SetScalarParameterValue("TileY", TileY);
			for (int32 i = 0; i < 6; ++i) {
				RailingStaticMeshComponent->SetMaterial(i, DynamicMaterial);
			}
		}
		
		RailingStaticMeshComponent->SetRelativeLocation(StartLocation);
		RailingStaticMeshComponent->SetRelativeRotation(StartRotation);
		RailingStaticMeshComponent->SetWorldScale3D(FVector{ FenceProperties.Length / 10, FenceProperties.Width / 10, FenceProperties.Height / 100});

		if (TopMeshType == ETopMeshType::GothicTop || TopMeshType == ETopMeshType::GothicStarTop) {
			RailingStaticMeshComponent->AddLocalRotation(FRotator{0.0,90.0,0.0});
		}
	
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

		FRotator StartRotation = SplineComponent->GetRotationAtDistanceAlongSpline(DistanceCovered, ESplineCoordinateSpace::World);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVerticalRailActor* SpawnedVerticalRailActor{};
		if (VerticalRailAssets && VerticalRailAssets->DataMap.Num() > 0) {
			if (auto VerticalRailActorClass = VerticalRailAssets->DataMap[TopMeshType].ClassRef) {
				SpawnedVerticalRailActor = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailActorClass, StartLocation, StartRotation, SpawnParams);
			}
			else {
				SpawnedVerticalRailActor = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), StartLocation, StartRotation, SpawnParams);
			}
		}

		if (SpawnedVerticalRailActor) {
			SpawnedVerticalRailActor->CreateVerticalRailActor(FVector{ FenceProperties.Length, FenceProperties.Width, FenceProperties.Height });
			if (SpawnedVerticalRailActor->TopMeshType == ETopMeshType::GothicTop || SpawnedVerticalRailActor->TopMeshType == ETopMeshType::GothicStarTop) {
				SpawnedVerticalRailActor->AddActorLocalRotation(FRotator{0.0,90.0,0.0});
			}
		}

		if (DynamicMaterial) {
			float TileX = (FenceProperties.Length + FenceProperties.Width) / 20.0f;
			float TileY = FenceProperties.Height / 50.0f;
			DynamicMaterial->SetScalarParameterValue("TileX", TileX);
			DynamicMaterial->SetScalarParameterValue("TileY", TileY);
			for (int32 i = 0; i < 6; ++i) {
				SpawnedVerticalRailActor->ProceduralMeshComponent->SetMaterial(i, DynamicMaterial);
			}
		}

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

		FRotator BeamStartRotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector BeamStartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);

		FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(BeamStartRotation);
		FVector RightDirection = UKismetMathLibrary::GetRightVector(BeamStartRotation);
		FVector UpDirection = UKismetMathLibrary::GetUpVector(BeamStartRotation);

		BeamStartLocation += (ForwardDirection * (Distance / 2));
		BeamStartLocation += (-RightDirection * ((FenceProperties.Width + 3) / 2));
		BeamStartLocation += (UpDirection * (3 * FenceProperties.Height / 4));

		HorizontalBeamStaticMeshComponentUp->SetRelativeRotation(BeamStartRotation);
		HorizontalBeamStaticMeshComponentUp->SetRelativeLocation(BeamStartLocation);
		HorizontalBeamStaticMeshComponentUp->SetWorldScale3D(FVector{ (Distance + 16) / 100, FenceProperties.Width / 10, FenceProperties.Height/100 });
	
		//Down
		UStaticMeshComponent* HorizontalBeamStaticMeshComponentDown = NewObject<UStaticMeshComponent>(this);
		HorizontalBeamStaticMeshComponentDown->RegisterComponent();
		HorizontalBeamStaticMeshComponentDown->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		HorizontalBeamStaticComponents.Add(HorizontalBeamStaticMeshComponentDown);

		if (HorizontalBeamStaticMesh) {
			HorizontalBeamStaticMeshComponentDown->SetStaticMesh(HorizontalBeamStaticMesh);
		}
		BeamStartLocation += (-UpDirection * (3 * FenceProperties.Height / 4));
		BeamStartLocation += (UpDirection * (FenceProperties.Height / 3));

		HorizontalBeamStaticMeshComponentDown->SetRelativeRotation(BeamStartRotation);
		HorizontalBeamStaticMeshComponentDown->SetRelativeLocation(BeamStartLocation);
		HorizontalBeamStaticMeshComponentDown->SetWorldScale3D(FVector{ (Distance + 16) / 100, FenceProperties.Width / 10, FenceProperties.Height/ 100 });

		if (DynamicMaterial) {
			float TileX = (FenceProperties.Length + FenceProperties.Width) / 20.0f;
			float TileY = FenceProperties.Height / 50.0f;
			DynamicMaterial->SetScalarParameterValue("TileX", TileX);
			DynamicMaterial->SetScalarParameterValue("TileY", TileY);
			HorizontalBeamStaticMeshComponentUp->SetMaterial(0, DynamicMaterial);
			HorizontalBeamStaticMeshComponentDown->SetMaterial(0, DynamicMaterial);
		}
	}
}

void AFenceMeshActor::GenerateProceduralHorizontalBeam() {
	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();

	for (int32 i = 0; i < SplinePointsCount - 1; ++i) {
		float Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i + 1) - SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);
		
		FRotator StartRotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);

		FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(StartRotation);
		FVector RightDirection = UKismetMathLibrary::GetRightVector(StartRotation);
		FVector UpDirection = UKismetMathLibrary::GetUpVector(StartRotation);

		// Up
		StartLocation += (ForwardDirection * (Distance / 2));
		StartLocation += (-RightDirection * ((FenceProperties.Width + 3) / 2));
		StartLocation += (UpDirection * (3 * FenceProperties.Height / 4));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVerticalRailActor* HorizontalBeamActorUp = GetWorld()->SpawnActor<AVerticalRailActor>(StartLocation, StartRotation, SpawnParams);
		HorizontalBeamActorUp->GenerateCube(0, FVector{Distance + 16, (3*FenceProperties.Width)/10 , (10*FenceProperties.Height) / 100}, 0.0f);
		
		// Down
		StartLocation += (-UpDirection * (3 * FenceProperties.Height / 4));
		StartLocation += (UpDirection * (FenceProperties.Height / 3));

		AVerticalRailActor* HorizontalBeamActorDown = GetWorld()->SpawnActor<AVerticalRailActor>(StartLocation, StartRotation, SpawnParams);
		HorizontalBeamActorDown->GenerateCube(0, FVector{ Distance + 16, (3 * FenceProperties.Width) / 10 , (10 * FenceProperties.Height) / 100 }, 0.0f);

		if (DynamicMaterial) {
			float TileX = (FenceProperties.Length + FenceProperties.Width) / 20.0f;
			float TileY = FenceProperties.Height / 50.0f;
			DynamicMaterial->SetScalarParameterValue("TileX", TileX);
			DynamicMaterial->SetScalarParameterValue("TileY", TileY);
			HorizontalBeamActorUp->ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
			HorizontalBeamActorDown->ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
		}
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
