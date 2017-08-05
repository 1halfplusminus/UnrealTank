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
	 // Get world location of linetrace through crosshair
	 // If it hits the landscape
	if(GetSightRayHitLocation(HitLocation))
	{
		// Tell controlled tank to aim at this point
		GetControlledTank()->AimAt(HitLocation);
	}
}

// Get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector &HitLocation) const
{
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		return false;
	}
	// Line trace along that look direction and see what we hit (up to max range)
	FHitResult HitResult;
	if (GetLookVectorHit(HitResult))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	else {
		return false;
	}
	
}

bool ATankPlayerController::GetLookVectorHit(FHitResult &HitResult) const
{
	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		ECC_Visibility);
}

void ATankPlayerController::DrawReachDebugLine() const
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

bool ATankPlayerController::GetCrossHairWorldDirection(FVector &LookDirection) const
{
	int32 ViewportSizeX, ViewPortSizeY;
	GetViewportSize(ViewportSizeX, ViewPortSizeY);
	FVector2D ScreenLocation(CrossHairXlocation * ViewportSizeX, CrossHairYLocation * ViewPortSizeY);
	FVector WorldLocation; //discard
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);

}

FVector ATankPlayerController::GetReachLineEnd() const
{

	
	FVector LookDirection;
	GetCrossHairWorldDirection(LookDirection);
	FVector LineTraceEnd = GetReachLineStart() + LookDirection  * LineTraceRange;
	return LineTraceEnd;
}

FVector ATankPlayerController::GetReachLineStart() const
{
	return PlayerCameraManager->GetCameraLocation();
}
