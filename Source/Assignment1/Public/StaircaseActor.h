// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaircaseActor.generated.h"

UCLASS()
class ASSIGNMENT1_API AStaircaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AStaircaseActor();

	virtual void Tick(float DeltaTime) override;
	void OnConstruction(const FTransform& Transform);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Staircase Properties")
	int32 NumberOfStaircases;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "Staircase Properties")
	TArray<UStaticMeshComponent*> StaircaseComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties")
	FVector Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties")
	UStaticMesh* StairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties")
	bool HasRailings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stair Properties", meta = (EditCondition = "HasRailings"))
	UStaticMesh* RailingMesh;

protected:
	virtual void BeginPlay() override;
};
