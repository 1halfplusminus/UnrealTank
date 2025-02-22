// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	
}
void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossedTankDeath);
	}
}
void ATankAIController::OnPossedTankDeath()
{
	auto PossesedPawn = GetPawn();
	if (PossesedPawn)
	{
		PossesedPawn->DetachFromControllerPendingDestroy();
	}
}
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (PlayerTank && ControlledTank) { 
		MoveToActor(PlayerTank, AcceptanceRadius);
		auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		if (AimingComponent)
		{
			AimingComponent->AimAt(PlayerTank->GetActorLocation());
			AimingComponent->Fire();
		}
	}
}