// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

class UPrimitiveComponent;
/**
 * TankTrack is used to set maxinum driving force, and to apply forces to the tank.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	// Sets a throttle between -1 and +1
	UFUNCTION(BlueprintCallable,Category= Input)
	void SetThrottle(float Throttle);

	// Max force per track, in centi Newtows
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 400000; // Assume 40 tonne tank, and 1g accelleration
private:
	UTankTrack();
	virtual void BeginPlay() override;
	void ApplySidewaysForce();
	void DriveTrack();
	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent,AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

	float CurrentThrottle = 0.0f;
};
