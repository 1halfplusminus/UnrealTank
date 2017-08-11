// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include  "TankBarrel.h"
#include  "TankTurret.h"
#include  "Projectile.h"
// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	bool CanShoot = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (CanShoot)
	{
		FiringState = EFiringStatus::Aiming;
	}
	else {
		FiringState = EFiringStatus::Reloading;
	}
}
void UTankAimingComponent::AimAt(FVector WorldSpaceAim)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return;  } // TODO Refractor

	FVector OutLaunchVelocity;
	auto StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	//Calculate out
	auto bCanAimAtWorldSpaceAim = UGameplayStatics::BlueprintSuggestProjectileVelocity(
		GetWorld(),
		OutLaunchVelocity,
		StartLocation,
		WorldSpaceAim,
		LaunchSpeed,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		0,
		false,
		false
	);
	if(bCanAimAtWorldSpaceAim)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }
	// Work-out difference between current barrel rotation , and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	// Move the barrel the right amount this frame
	// Given a max elevation speed, and the frame time
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 100)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else {
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	auto Barrel = GetOwner()->FindComponentByClass<UTankBarrel>();
	if (!ensure(Barrel)) { return; }
	if (FiringState == EFiringStatus::Aiming) {
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
		if (Projectile)
		{
			Projectile->Launch(LaunchSpeed);
			LastFireTime = FPlatformTime::Seconds();
		}
	}
}