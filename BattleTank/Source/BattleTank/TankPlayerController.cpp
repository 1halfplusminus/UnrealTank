// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"

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

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}


void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; //Out parameter
	if(GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString());
	}
	DrawReachDebugLine();
	// Get world location of linetrace through crosshair
	// If it hits the landscape
		// Telle controlled tank to aim at this point
}

// Get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector &HitLocation) const
{
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		return false;
	}
	FVector StartLocation;
	FRotator ActorRotation;
	FHitResult HitResult;
	ControlledTank->GetActorEyesViewPoint(StartLocation, ActorRotation);
	if (GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		StartLocation,
		StartLocation + RotationInput.Vector() * 1000,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	else {
		return false;
	}
	
}

void ATankPlayerController::DrawReachDebugLine()
{
	DrawDebugLine(
		GetWorld(),
		GetReachLineStart(),
		GetReachLineEnd(),
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);
}

FVector ATankPlayerController::GetReachLineEnd()
{
	FVector Location;
	FRotator Rotation;
	FRotator SpringArmRotation;
	ATank*   Tank = GetControlledTank();
	USceneComponent* AzimuteGimble = Tank->FindComponentByClass<USceneComponent>();

	FVector LineTraceEnd = GetControlledTank()->GetActorLocation() + Rotation.Vector()  * 100000000;
	return LineTraceEnd;
}

FVector ATankPlayerController::GetReachLineStart()
{
	return GetControlledTank()->GetActorLocation();
}
