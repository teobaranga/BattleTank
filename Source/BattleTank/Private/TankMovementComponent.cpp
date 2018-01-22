// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Components/StaticMeshComponent.h"

const FName UTankMovementComponent::TrackForceSocketName = FName("Force");

void UTankMovementComponent::Initialize(UStaticMeshComponent* Tank,
                                        UStaticMeshComponent* LeftTrack, UStaticMeshComponent* RightTrack)
{
    this->Tank = Tank;
    this->LeftTrack = LeftTrack;
    this->RightTrack = RightTrack;
}

void UTankMovementComponent::MoveForwardIntent(float Throw)
{
    if (!Tank || !LeftTrack || !RightTrack)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing tank or tracks, cannot move forward"));
        return;
    }
    /// TODO: clamp throttle value

    FVector ForceApplied = LeftTrack->GetRightVector() * Throw * MaxDrivingForce;
    FVector ForceLocation = LeftTrack->GetSocketLocation(TrackForceSocketName);
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);

    ForceApplied = RightTrack->GetRightVector() * Throw * MaxDrivingForce;
    ForceLocation = RightTrack->GetSocketLocation(TrackForceSocketName);
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankMovementComponent::TurnRightIntent(float Throw)
{
    if (!Tank || !LeftTrack || !RightTrack)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing tank or tracks, cannot turn"));
        return;
    }

    FVector ForceApplied = LeftTrack->GetRightVector() * Throw * MaxDrivingForce;
    FVector ForceLocation = LeftTrack->GetSocketLocation(TrackForceSocketName);
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);

    ForceApplied = RightTrack->GetRightVector() * -Throw * MaxDrivingForce;
    ForceLocation = RightTrack->GetSocketLocation(TrackForceSocketName);
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    UE_LOG(LogTemp, Warning, TEXT("RequestDirectMove: %s"), *MoveVelocity.ToCompactString());
}
