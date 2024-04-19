// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

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

		if (HasRailings) {
			FString LeftRailingName = StairComponentName + " Left Railing";
			FString RightRailingName = StairComponentName + " Right Railing";

			StairComponent.LeftRailing = NewObject<UStaticMeshComponent>(StairComponent.Stair, UStaticMeshComponent::StaticClass(), *LeftRailingName);
			StairComponent.RightRailing = NewObject<UStaticMeshComponent>(StairComponent.Stair, UStaticMeshComponent::StaticClass(), *RightRailingName);

			if (RailingMesh) {
				StairComponent.LeftRailing->SetStaticMesh(RailingMesh);
				StairComponent.RightRailing->SetStaticMesh(RailingMesh);
				RailingMeshSize = RailingMesh->GetBounds().GetBox().GetSize();
			}

			StairComponent.LeftRailing->AttachToComponent(StairComponent.Stair, FAttachmentTransformRules::KeepRelativeTransform);
			StairComponent.LeftRailing->RegisterComponentWithWorld(GetWorld());

			StairComponent.RightRailing->AttachToComponent(StairComponent.Stair, FAttachmentTransformRules::KeepRelativeTransform);
			StairComponent.RightRailing->RegisterComponentWithWorld(GetWorld());

			

			if (StairCaseType == EStaircaseType::BoxStaircase) {
				FVector ParentDimensions = StairComponent.Stair->GetRelativeScale3D();
				FVector RailingTotalSize{ RailingDimensions.X * RailingMeshSize.X, RailingDimensions.Y * RailingMeshSize.Y, (ParentDimensions.Z + (StairDimensions.Z* (RailingDimensions.Z - 1))) * RailingMeshSize.Z };

				StairComponent.LeftRailing->SetWorldScale3D(FVector(RailingDimensions.X, RailingDimensions.Y, ParentDimensions.Z + (StairDimensions.Z * (RailingDimensions.Z-1))));
				StairComponent.RightRailing->SetWorldScale3D(FVector(RailingDimensions.X, RailingDimensions.Y, ParentDimensions.Z + (StairDimensions.Z * (RailingDimensions.Z-1))));
				StairComponent.LeftRailing->SetRelativeLocation(FVector(0, -((((StairTotalSize.Y - RailingTotalSize.Y) / (2 * StairTotalSize.Y)) + (RailingTotalSize.Y / StairTotalSize.Y)) * 100), (((RailingTotalSize.Z - (StairTotalSize.Z * (i+1))) / (2 * StairTotalSize.Z * (i + 1))) * 100)));
				StairComponent.RightRailing->SetRelativeLocation(FVector(0, ((((StairTotalSize.Y - RailingTotalSize.Y) / (2 * StairTotalSize.Y)) + (RailingTotalSize.Y / StairTotalSize.Y)) * 100), (((RailingTotalSize.Z - (StairTotalSize.Z * (i+1))) / (2 * StairTotalSize.Z * (i + 1))) * 100)));
			}
			else {
				FVector RailingTotalSize{ RailingDimensions.X * StairDimensions.X * RailingMeshSize.X, RailingDimensions.Y * StairDimensions.Y * RailingMeshSize.Y, RailingDimensions.Z * StairDimensions.Z * RailingMeshSize.Z };
				
				StairComponent.LeftRailing->SetRelativeScale3D(RailingDimensions);
				StairComponent.RightRailing->SetRelativeScale3D(RailingDimensions);

				StairComponent.LeftRailing->SetRelativeLocation(FVector(0, -((((StairTotalSize.Y - RailingTotalSize.Y) / (2 * StairTotalSize.Y)) + (RailingTotalSize.Y / StairTotalSize.Y)) * 100), (((RailingTotalSize.Z - StairTotalSize.Z) / (2 * StairTotalSize.Z)) * 100)));
				StairComponent.RightRailing->SetRelativeLocation(FVector(0, ((((StairTotalSize.Y - RailingTotalSize.Y) / (2 * StairTotalSize.Y)) + (RailingTotalSize.Y / StairTotalSize.Y)) * 100), (((RailingTotalSize.Z - StairTotalSize.Z) / (2 * StairTotalSize.Z)) * 100)));
			}
		}

		StairComponents.Add(StairComponent);
	}
}

void AStaircaseActor::OnConstruction(const FTransform& Transform) {
	UE_LOG(LogTemp, Warning, TEXT("On Construction Called"));

	DestroyStaircase();

	CreateStairs();
}

FStairComponent::FStairComponent() : Stair{ nullptr }, LeftRailing{ nullptr }, RightRailing{ nullptr } {}