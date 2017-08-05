// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
*
*/
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float) override;
private:

	UPROPERTY(EditAnywhere)
	float CrossHairXlocation = 0.5f;
	UPROPERTY(EditAnywhere)
	float CrossHairYLocation = 0.33333f;

	FVector GetReachLineEnd();
	FVector GetReachLineStart();
	ATank* GetControlledTank() const;
	bool GetSightRayHitLocation(FVector&) const;

	void DrawReachDebugLine();
	// Start the tank moving the barrel so that a shot would hit where 
	// the crosshair intersects the world
	void   AimTowardsCrosshair();

};