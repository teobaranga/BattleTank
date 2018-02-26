// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

/**
 * Component responsible for driving the tank tracks
 */
UCLASS(ClassGroup = (Tank), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialize(UStaticMeshComponent* Tank, UStaticMeshComponent* LeftTrack, UStaticMeshComponent* RightTrack);

    // Set the tracks throttle to a value between -1 and 1
    UFUNCTION(BlueprintCallable, Category = "Input")
    void MoveForwardIntent(float Throw);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void TurnRightIntent(float Throw);

    // Function called indirectly by the AI controller, which provides info needed to move to the requested target
    void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

    void DriveTracks();

    void ApplySidewaysForce();

private:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    // Max force per track, in Newtons
    UPROPERTY(EditDefaultsOnly)
    float MaxDrivingForce = 4600000.0f * 9.0f; // 4.5 m/s^2

    float CurrentThrottle = 0.f;

    // Name of the socket whose location will be used to apply the tracks force
    static const FName TrackForceSocketName;

    UStaticMeshComponent* Tank;
    UStaticMeshComponent* LeftTrack;
    UStaticMeshComponent* RightTrack;
};
