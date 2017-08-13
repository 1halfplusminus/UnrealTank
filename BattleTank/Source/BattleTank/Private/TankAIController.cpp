// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
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
			if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
			{
				AimingComponent->Fire();
			}
		}
	}
}