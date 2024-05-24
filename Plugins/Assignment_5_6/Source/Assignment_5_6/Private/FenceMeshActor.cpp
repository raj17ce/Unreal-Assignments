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

	SplineComponent->ClearSplinePoints();
}

// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AFenceMeshActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}
