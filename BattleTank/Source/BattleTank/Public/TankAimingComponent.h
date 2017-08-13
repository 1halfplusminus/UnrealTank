// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//ENUM
UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Locked,
	Aiming
};
// Forward Declaration
class UTankBarrel; 
class UTankTurret;
class AProjectile;

// Responsible of balistic calculation
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UTankAimingComponent();
	void AimAt(FVector HitLocation);
	UFUNCTION(BlueprintCallable)
	void Fire();
	UFUNCTION(BlueprintCallable,Category="Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	EFiringStatus GetFiringState() const;
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	void MoveBarrelTowards(FVector AimDirection);
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	TSubclassOf<AProjectile> ProjectileBlueprint; // alternative
	float LastFireTime = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	EFiringStatus FiringState = EFiringStatus::Reloading;

	FVector AimDirection = FVector(0);
private:
	bool IsBarrelMoving();
};
