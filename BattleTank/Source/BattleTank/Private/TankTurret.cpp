// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawRotationElevation = RelativeRotation.Yaw + RotationChange;

	SetRelativeRotation(FRotator(0.0f, FMath::Clamp<float>(RawRotationElevation, MinRotationDegrees, MaxRotationDegrees),0.0f));
}