// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"



ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
void ATankAIController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AI BEGIN PLAY"));
	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI CONTROLLING: %s"), *ControlledTank->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MOUAHAHAHAHAH"));
	}
}