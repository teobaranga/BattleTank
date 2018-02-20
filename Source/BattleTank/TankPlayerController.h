// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

/**
 * Responsible for helping the player aim.
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
    GENERATED_BODY()
    
public:
    // Note: must match UI
    UPROPERTY(EditDefaultsOnly)
    float CrosshairXLocation = 0.5f;

    // Note: must match UI
    UPROPERTY(EditDefaultsOnly)
    float CrosshairYLocation = 1 / 3.0f;

    /* Line trace range, in meters. Default: 10km */
    UPROPERTY(EditDefaultsOnly)
    float LineTraceRange = 1000000.0f;

    virtual void BeginPlay() override;

    virtual void PlayerTick(float DeltaTime) override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnPossess(UTankAimingComponent* TankAimingComponent);

    UFUNCTION(BlueprintImplementableEvent)
    void OnUnPossess();

protected:
    /** Pawn has been possessed, so changing state to NAME_Playing. Start it walking and begin playing with it. */
    virtual void BeginPlayingState() override;

    /** Leave playing state. */
    virtual void EndPlayingState() override;
    
private:
    /* 
    Aim the tank barrel so that a shot fired would land where the crosshair
    intersects the world
    */
    void AimTowardsCrosshair();

    /* Get world location of linetrace through crosshair */
    bool GetSightRayHitLocation(FVector& OutHitLocation) const;

    /* The aiming component of the player tank */
    UTankAimingComponent* TankAimingComponent = nullptr;

};
