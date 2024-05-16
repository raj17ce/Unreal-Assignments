// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchMeshActor.h"

AArchMeshActor::AArchMeshActor() {
	RootStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");

	SetRootComponent(RootStaticMeshComponent);
}