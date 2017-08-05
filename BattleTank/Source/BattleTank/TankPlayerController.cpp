// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("PLAYER CONTROLLER BEGIN PLAY"));
	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("YOUR ARE CONTROLLING: %s"),*ControlledTank->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MOUAHAHAHAHAH"));
	}
}
