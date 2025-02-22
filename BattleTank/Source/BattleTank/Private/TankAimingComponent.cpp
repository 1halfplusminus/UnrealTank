// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include  "TankBarrel.h"
#include  "TankTurret.h"
#include  "Projectile.h"
#include  "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	NumberOfAmmo = StartAmmo;
}
void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}
EFiringStatus UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}
int32 UTankAimingComponent::GetNumberOfAmmo() const
{
	return NumberOfAmmo;
}
bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection,0.01);
}
void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	bool CanShoot = (GetWorld()->GetTimeSeconds() - LastFireTime) > ReloadTimeInSeconds;
	if (NumberOfAmmo <= 0)
	{
		FiringState = EFiringStatus::OutOfAmmo;
	}
	else if(!CanShoot)
	{
		FiringState = EFiringStatus::Reloading;
	}
	else if(IsBarrelMoving()) {
		FiringState = EFiringStatus::Aiming;
	}
	else {
		FiringState = EFiringStatus::Locked;
	}
}
void UTankAimingComponent::AimAt(FVector WorldSpaceAim)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return;  } // TODO Refractor

	FVector OutLaunchVelocity;
	auto StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	//Calculate out
	auto bCanAimAtWorldSpaceAim = UGameplayStatics::BlueprintSuggestProjectileVelocity(
		this,
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
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(FMath::FindDeltaAngleDegrees( BarrelRotator.Yaw, AimAsRotator.Yaw));
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringStatus::Aiming || FiringState == EFiringStatus::Locked) {
		auto Barrel = GetOwner()->FindComponentByClass<UTankBarrel>();
		if (!ensure(Barrel) || !ensure(ProjectileBlueprint)) { return; }
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
		if (Projectile)
		{
			Projectile->Launch(LaunchSpeed);
			LastFireTime = GetWorld()->GetTimeSeconds();
			NumberOfAmmo--;
		}
	}
}