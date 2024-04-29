// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignment3.h"

#define LOCTEXT_NAMESPACE "FAssignment3Module"

void FAssignment3Module::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAssignment3Module::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssignment3Module, Assignment3)