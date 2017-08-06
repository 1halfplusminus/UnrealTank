// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Tank.h"

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
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("TANK AI BEGIN PLAY"));
	auto ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("A AI ARE CONTROLLING: %s"), *ControlledTank->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AN AI IS BUGGING"));
	}
}
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsThereAPlayer() || !GetControlledTank()) { return; }
	auto PlayerTank = GetPlayerTank();
	GetControlledTank()->AimAt(PlayerTank->GetActorLocation());
}