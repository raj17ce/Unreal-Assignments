// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicPawnController.h"

ADynamicPawnController::ADynamicPawnController() : CurrentPawn{ nullptr }, CurrentPawnIndex{0} {

	PawnDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Assignment2/Data/BP_PawnDataTable.BP_PawnDataTable'"));
	PawnNames = PawnDataTable->GetRowNames();
}

void ADynamicPawnController::SetupInputComponent() {

	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (SwapAction) {
			EIC->BindAction(SwapAction, ETriggerEvent::Completed, this, &ADynamicPawnController::SpawnPawn);
		}
	}

	if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		if (PawnMappingContext) {
			LocalPlayerSubsystem->AddMappingContext(PawnMappingContext, 0);
		}
	}
}

void ADynamicPawnController::BeginPlay() {
	SpawnPawn();
}

void ADynamicPawnController::SpawnPawn() {
	if (PawnDataTable) {
		FPawnDataTable* CurrentRow = PawnDataTable->FindRow<FPawnDataTable>(PawnNames[CurrentPawnIndex], TEXT("Current Row"));

		if (CurrentRow) {

			FVector CurrentPawnSpawnLocation;
			FRotator CurrentPawnSpawnRotation;
			if (CurrentPawn) {
				CurrentPawnSpawnLocation = CurrentPawn->GetActorLocation();
				CurrentPawnSpawnRotation = CurrentPawn->GetActorRotation();
				CurrentPawn->Destroy();
				CurrentPawn = nullptr;
			}
			else {
				CurrentPawnSpawnLocation = FVector(0,0,50);
				CurrentPawnSpawnRotation = FRotator(0, 0, 0);
			}	

			FActorSpawnParameters CurrentPawnSpawnParams;
			CurrentPawnSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
			
			if (LocalPlayerSubsystem) {
				LocalPlayerSubsystem->ClearAllMappings();
			}

			APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CurrentRow->PawnClass, CurrentPawnSpawnLocation, CurrentPawnSpawnRotation, CurrentPawnSpawnParams);
			
			if (LocalPlayerSubsystem) {
				LocalPlayerSubsystem->AddMappingContext(PawnMappingContext, 0);
			}

			if(SpawnedPawn) {
				Possess(SpawnedPawn);
		
				CurrentPawn = SpawnedPawn;
				CurrentPawnType = CurrentRow->PawnType;
				CurrentPawnIndex = (CurrentPawnIndex + 1) % PawnNames.Num();

				if (CurrentPawnType == EPawnType::TopDownPawn) {
					SetShowMouseCursor(true);
				}
				else {
					SetShowMouseCursor(false);
				}
			}
		}
	}
}