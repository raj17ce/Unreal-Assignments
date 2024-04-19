// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaircaseActor.generated.h"

UENUM()
enum class EStaircaseType : uint8 {
	OpenStaircase,
	CloseStaircase,
	BoxStaircase
};

USTRUCT()
struct FStairComponent {

public:
	GENERATED_USTRUCT_BODY()

	UStaticMeshComponent* Stair;
	UStaticMeshComponent* LeftRailing;
	UStaticMeshComponent* RightRailing;
};

UCLASS()
class ASSIGNMENT1_API AStaircaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AStaircaseActor();

	virtual void Tick(float DeltaTime) override;
	//void DestroyStairCase();
	void CreateStair();
	void OnConstruction(const FTransform& Transform);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Staircase Properties")
	int32 NumberOfStairs;

	UPROPERTY(VisibleDefaultsOnly, category = "Staircase Properties")
	TArray<FStairComponent> StairComponents;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Staircase Properties")
	EStaircaseType StairCaseType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties")
	FVector StairDimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties")
	FVector StairTranslationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties")
	UStaticMesh* StairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties")
	bool HasRailings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties", meta = (EditCondition = "HasRailings"))
	UStaticMesh* RailingMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Stair Properties", meta = (EditCondition = "HasRailings"))
	FVector RailingDimensions;

protected:
	virtual void BeginPlay() override;
};
