// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

/**
 * Component responsible for driving the tank tracks
 */
UCLASS()
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable)
    void Initialize(UStaticMeshComponent* Tank, UStaticMeshComponent* LeftTrack, UStaticMeshComponent* RightTrack);

    // Set the tracks throttle to a value between -1 and 1
    UFUNCTION(BlueprintCallable)
    void MoveForwardIntent(float Throw);

    UFUNCTION(BlueprintCallable)
    void TurnRightIntent(float Throw);

    void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

private:
    // Max force per track, in Newtons
    UPROPERTY(EditDefaultsOnly)
    float MaxDrivingForce = 4600000.0f * 9.0f; // 4.5 m/s^2

    // Name of the socket whose location will be used to apply the tracks force
    static const FName TrackForceSocketName;

    UStaticMeshComponent* Tank;
    UStaticMeshComponent* LeftTrack;
    UStaticMeshComponent* RightTrack;
};
