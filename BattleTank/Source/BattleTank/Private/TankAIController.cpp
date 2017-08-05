// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
ATank * ATankAIController::GetPlayerTank() const
{
	if (!IsThereAPlayer())
	{
		return nullptr;
	}
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
bool ATankAIController::IsThereAPlayer() const
{
	return GetWorld()->GetFirstPlayerController() != nullptr;
}
void ATankAIController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AI BEGIN PLAY"));
	auto PlayerTank = GetPlayerTank();
	if (PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMING AT: %s"), *PlayerTank->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CAN'T FIND THE TARGET"));
	}
}