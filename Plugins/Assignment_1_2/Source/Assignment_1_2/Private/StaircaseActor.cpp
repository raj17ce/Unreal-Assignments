// Fill out your copyright notice in the Description page of Project Settings.

#include "StaircaseActor.h"
#include <math.h>

AStaircaseActor::AStaircaseActor() : NumberOfStairs{}, StairDimensions{}, StairMesh{}, HasRailings{}, RailingMesh{}, RailingDimensions{}
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

void AStaircaseActor::DestroyStaircase() {

	for (int32 i = 0; i < StairComponents.Num(); ++i) {
		if (StairComponents[i].LeftRailingLine) {
			StairComponents[i].LeftRailingLine->DestroyComponent();
			StairComponents[i].LeftRailingLine = nullptr;
		}
		if (StairComponents[i].RightRailingLine) {
			StairComponents[i].RightRailingLine->DestroyComponent();
			StairComponents[i].RightRailingLine = nullptr;
		}
		if (StairComponents[i].LeftRailing) {
			StairComponents[i].LeftRailing->DestroyComponent();
			StairComponents[i].LeftRailing = nullptr;
		}
		if (StairComponents[i].RightRailing) {
			StairComponents[i].RightRailing->DestroyComponent();
			StairComponents[i].RightRailing = nullptr;
		}
		if (StairComponents[i].Stair) {
			StairComponents[i].Stair->DestroyComponent();
			StairComponents[i].Stair = nullptr;
		}
	}

	StairComponents.Empty();
}

void AStaircaseActor::CreateStairs() {
	for (int32 i = 0; i < NumberOfStairs; ++i) {

		FString StairComponentName = "Stair" + FString::FromInt(i);
		FStairComponent StairComponent;
		StairComponent.Stair = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *StairComponentName);

		FVector StairMeshSize;

		if (StairMesh) {
			StairComponent.Stair->SetStaticMesh(StairMesh);
			StairMeshSize = StairMesh->GetBounds().GetBox().GetSize();

			if (StairMeshMaterial) {
				StairComponent.Stair->SetMaterial(0, StairMeshMaterial);
			}
		}

		FVector StairTotalSize{ StairDimensions.X * StairMeshSize.X, StairDimensions.Y * StairMeshSize.Y , StairDimensions.Z * StairMeshSize.Z };

		if (StairCaseType == EStaircaseType::BoxStaircase) {
			StairComponent.Stair->SetRelativeScale3D(FVector(StairDimensions.X, StairDimensions.Y, StairDimensions.Z * (i + 1)));
			StairComponent.Stair->SetRelativeLocation(FVector((StairTotalSize.X * (StairTranslationOffset.X / 100) * i), 0, (StairTotalSize.Z * (i + 1) * (StairTranslationOffset.Z / 100))));
		}
		else {
			StairComponent.Stair->SetRelativeScale3D(StairDimensions);
			StairComponent.Stair->SetRelativeLocation(FVector((StairTotalSize.X * (StairTranslationOffset.X / 100) * i), 0, (StairTotalSize.Z * (StairTranslationOffset.Z / 100) * i)));
		}
		StairComponent.Stair->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StairComponent.Stair->RegisterComponentWithWorld(GetWorld());

		FVector RailingMeshSize;
		FVector RailingLineMeshSize;

		if (HasRailings) {
			FString LeftRailingName = StairComponentName + " Left Railing";
			FString RightRailingName = StairComponentName + " Right Railing";

			StairComponent.LeftRailing = NewObject<UStaticMeshComponent>(StairComponent.Stair, UStaticMeshComponent::StaticClass(), *LeftRailingName);
			StairComponent.RightRailing = NewObject<UStaticMeshComponent>(StairComponent.Stair, UStaticMeshComponent::StaticClass(), *RightRailingName);

			if (RailingMesh) {
				StairComponent.LeftRailing->SetStaticMesh(RailingMesh);
				StairComponent.RightRailing->SetStaticMesh(RailingMesh);
				RailingMeshSize = RailingMesh->GetBounds().GetBox().GetSize();

				if (RailingMeshMaterial) {
					StairComponent.LeftRailing->SetMaterial(0, RailingMeshMaterial);
					StairComponent.RightRailing->SetMaterial(0, RailingMeshMaterial);
				}
			}

			StairComponent.LeftRailing->AttachToComponent(StairComponent.Stair, FAttachmentTransformRules::KeepRelativeTransform);
			StairComponent.LeftRailing->RegisterComponentWithWorld(GetWorld());

			StairComponent.RightRailing->AttachToComponent(StairComponent.Stair, FAttachmentTransformRules::KeepRelativeTransform);
			StairComponent.RightRailing->RegisterComponentWithWorld(GetWorld());

			FVector RailingTotalSize{ RailingDimensions.X * StairDimensions.X * RailingMeshSize.X, RailingDimensions.Y * StairDimensions.Y * RailingMeshSize.Y, RailingDimensions.Z * StairDimensions.Z * RailingMeshSize.Z };
			
			if (StairCaseType == EStaircaseType::BoxStaircase) {
				FVector ParentDimensions = StairComponent.Stair->GetRelativeScale3D();
				RailingTotalSize = FVector(RailingDimensions.X * RailingMeshSize.X, RailingDimensions.Y * RailingMeshSize.Y, RailingDimensions.Z * RailingMeshSize.Z);

				StairComponent.LeftRailing->SetWorldScale3D(RailingDimensions);
				StairComponent.RightRailing->SetWorldScale3D(RailingDimensions);
				StairComponent.LeftRailing->SetRelativeLocation(FVector(0, -((((StairTotalSize.Y / 2) - (RailingTotalSize.Y * 2)) / StairTotalSize.Y) * 100), ((((RailingTotalSize.Z / 2) + ((StairTotalSize.Z * (i+1)) / 2)) / (StairTotalSize.Z * (i+1))) * 100)));
				StairComponent.RightRailing->SetRelativeLocation(FVector(0, ((((StairTotalSize.Y / 2) - (RailingTotalSize.Y * 2)) / StairTotalSize.Y) * 100), ((((RailingTotalSize.Z / 2) + ((StairTotalSize.Z * (i+1)) / 2)) / (StairTotalSize.Z * (i+1))) * 100)));
			}
			else {
				StairComponent.LeftRailing->SetRelativeScale3D(RailingDimensions);
				StairComponent.RightRailing->SetRelativeScale3D(RailingDimensions);

				StairComponent.LeftRailing->SetRelativeLocation(FVector(0, -((((StairTotalSize.Y / 2) - (RailingTotalSize.Y * 2)) / StairTotalSize.Y) * 100), ((((RailingTotalSize.Z / 2) + (StairTotalSize.Z / 2)) / StairTotalSize.Z) * 100)));
				StairComponent.RightRailing->SetRelativeLocation(FVector(0, ((((StairTotalSize.Y / 2) - (RailingTotalSize.Y * 2)) / StairTotalSize.Y) * 100), ((((RailingTotalSize.Z / 2) + (StairTotalSize.Z / 2)) / StairTotalSize.Z) * 100)));
			}

			FString LeftRailingLineName = StairComponentName + " Left Railing Line";
			FString RightRailingLineName = StairComponentName + " Right Railing Line";

			StairComponent.LeftRailingLine = NewObject<UStaticMeshComponent>(StairComponent.LeftRailing, UStaticMeshComponent::StaticClass(), *LeftRailingLineName);
			StairComponent.RightRailingLine = NewObject<UStaticMeshComponent>(StairComponent.RightRailing, UStaticMeshComponent::StaticClass(), *RightRailingLineName);

			if (RailingLineMesh) {
				StairComponent.LeftRailingLine->SetStaticMesh(RailingLineMesh);
				StairComponent.RightRailingLine->SetStaticMesh(RailingLineMesh);
				RailingLineMeshSize = RailingLineMesh->GetBounds().GetBox().GetSize();

				if (RailingLineMeshMaterial) {
					StairComponent.LeftRailingLine->SetMaterial(0, RailingLineMeshMaterial);
					StairComponent.RightRailingLine->SetMaterial(0, RailingLineMeshMaterial);
				}
			}

			StairComponent.LeftRailingLine->AttachToComponent(StairComponent.LeftRailing, FAttachmentTransformRules::KeepRelativeTransform);
			StairComponent.LeftRailingLine->RegisterComponentWithWorld(GetWorld());

			StairComponent.RightRailingLine->AttachToComponent(StairComponent.RightRailing, FAttachmentTransformRules::KeepRelativeTransform);
			StairComponent.RightRailingLine->RegisterComponentWithWorld(GetWorld());

			StairComponent.LeftRailingLine->SetRelativeLocation(FVector(0, 0, 50));
			StairComponent.RightRailingLine->SetRelativeLocation(FVector(0, 0, 50));

			double LengthX{ 0 };
			double LengthZ{ 0 };

			if (StairCaseType == EStaircaseType::BoxStaircase) {
				StairComponent.LeftRailingLine->SetWorldScale3D(FVector((StairDimensions.X * (StairTranslationOffset.X / 100) * 1.2), (RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));
				StairComponent.RightRailingLine->SetWorldScale3D(FVector((StairDimensions.X * (StairTranslationOffset.X / 100) * 1.2), (RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));

				LengthX = StairDimensions.X;
				LengthZ = StairDimensions.Z;
			}
			else {
				StairComponent.LeftRailingLine->SetWorldScale3D(FVector((StairDimensions.X * (StairTranslationOffset.X / 100) * 1.2), (StairDimensions.Y * RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));
				StairComponent.RightRailingLine->SetWorldScale3D(FVector((StairDimensions.X * (StairTranslationOffset.X / 100) * 1.2), (StairDimensions.Y * RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));

				LengthX = StairDimensions.X + (((StairTranslationOffset.X / 100) - 1) * StairDimensions.X);
				LengthZ = StairDimensions.Z + (((StairTranslationOffset.Z / 100) - 1) * StairDimensions.Z);
			}

			double RailingLinePitch = atan(LengthZ / LengthX) * 180 / PI;

			if (StairCaseType == EStaircaseType::BoxStaircase) {
				StairComponent.LeftRailingLine->SetWorldScale3D(FVector((LengthX / (cos(RailingLinePitch * PI / 180))), (RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));
				StairComponent.RightRailingLine->SetWorldScale3D(FVector((LengthX / (cos(RailingLinePitch * PI / 180))), (RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));
			}
			else {
				StairComponent.LeftRailingLine->SetWorldScale3D(FVector((LengthX / (cos(RailingLinePitch * PI / 180))), (StairDimensions.Y * RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));
				StairComponent.RightRailingLine->SetWorldScale3D(FVector((LengthX / (cos(RailingLinePitch * PI / 180))), (StairDimensions.Y * RailingDimensions.Y * 3), (StairDimensions.Z * 0.2)));
			}

			StairComponent.LeftRailingLine->SetRelativeRotation(FRotator(RailingLinePitch, 0, 0));
			StairComponent.RightRailingLine->SetRelativeRotation(FRotator(RailingLinePitch, 0, 0));
		}

		StairComponents.Add(StairComponent);
	}
}

void AStaircaseActor::OnConstruction(const FTransform& Transform) {
	UE_LOG(LogTemp, Warning, TEXT("On Construction Called"));

	DestroyStaircase();

	CreateStairs();
}

FStairComponent::FStairComponent() : Stair{ nullptr }, LeftRailing{ nullptr }, RightRailing{ nullptr }, LeftRailingLine{ nullptr }, RightRailingLine{ nullptr } {}