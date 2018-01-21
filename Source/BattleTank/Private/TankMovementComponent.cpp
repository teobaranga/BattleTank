// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Components/StaticMeshComponent.h"


void UTankMovementComponent::Initialize(UStaticMeshComponent* Tank,
                                        UStaticMeshComponent* LeftTrack, UStaticMeshComponent* RightTrack)
{
    this->Tank = Tank;
    this->LeftTrack = LeftTrack;
    this->RightTrack = RightTrack;
}

void UTankMovementComponent::MoveForwardIntent(float Throw)
{
    UE_LOG(LogTemp, Warning, TEXT("MoveForwardIntent: %f"), Throw);

    /// TODO: clamp throttle value

    FVector ForceApplied = LeftTrack->GetRightVector() * Throw * MaxDrivingForce;
    FVector ForceLocation = LeftTrack->GetComponentLocation();
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);

    ForceApplied = RightTrack->GetRightVector() * Throw * MaxDrivingForce;
    ForceLocation = RightTrack->GetComponentLocation();
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);
}
