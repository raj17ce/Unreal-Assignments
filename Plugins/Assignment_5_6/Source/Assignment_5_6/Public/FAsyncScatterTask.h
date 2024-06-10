// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"

class AMeshGenerator;

/**
 * 
 */
class ASSIGNMENT_5_6_API FAsyncScatterTask : public FNonAbandonableTask {

public:
	FAsyncScatterTask(AMeshGenerator* NewMeshGenerator);

	void DoWork();

	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncScatterTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	TWeakObjectPtr<AMeshGenerator> MeshGenerator;

	FVector FindRandomPointInSphere(FVector Origin, const FVector& ShapeDimensions);
	FVector FindRandomPointInBox(FVector Origin, const FVector& ShapeDimensions);
};
