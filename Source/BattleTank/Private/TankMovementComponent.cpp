// Copyright (c) 2018 Teo Baranga

#include "TankMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

const FName UTankMovementComponent::TrackForceSocketName = FName("Force");

void UTankMovementComponent::Initialize(UStaticMeshComponent* Tank,
                                        UStaticMeshComponent* LeftTrack, UStaticMeshComponent* RightTrack)
{
    this->Tank = Tank;
    this->LeftTrack = LeftTrack;
    this->RightTrack = RightTrack;

    if (!Tank || !LeftTrack || !RightTrack)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing tank or tracks, movement component will not work"));
        return;
    }

    this->LeftTrack->OnComponentHit.AddDynamic(this, &UTankMovementComponent::OnHit);
    this->RightTrack->OnComponentHit.AddDynamic(this, &UTankMovementComponent::OnHit);
}

void UTankMovementComponent::MoveForwardIntent(float Throw)
{
    CurrentThrottle = FMath::Clamp(Throw, -1.f, 1.f);
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
    FVector TankForward = GetOwner()->GetActorRightVector().GetSafeNormal();
    FVector TankMoveDirection = MoveVelocity.GetSafeNormal();

    /// Move forward towards the player
    MoveForwardIntent(FVector::DotProduct(TankForward, TankMoveDirection));

    /// Rotate towards the player
    TurnRightIntent(FVector::CrossProduct(TankForward, TankMoveDirection).Z);
}

void UTankMovementComponent::DriveTracks()
{
    if (!Tank || !LeftTrack || !RightTrack)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing tank or tracks, cannot move forward"));
        return;
    }

    /// Forwards force
    FVector ForceApplied = Tank->GetRightVector() * CurrentThrottle * MaxDrivingForce;

    FVector ForceLocation = LeftTrack->GetSocketLocation(TrackForceSocketName);
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);

    ForceLocation = RightTrack->GetSocketLocation(TrackForceSocketName);
    Tank->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankMovementComponent::ApplySidewaysForce()
{
    // Compute the velocity sideways
    float SlipVelocity = FVector::DotProduct(Tank->GetComponentVelocity(), Tank->GetForwardVector());

    float DeltaTime = GetWorld()->GetDeltaSeconds();
    FVector CorrectionAcceleration = -SlipVelocity / DeltaTime * Tank->GetForwardVector();
    FVector CorrectionForce = Tank->GetMass() * CorrectionAcceleration;
    Tank->AddForce(CorrectionForce);
}

void UTankMovementComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // Drive the tracks
    DriveTracks();

    // Apply the correction sideways force
    ApplySidewaysForce();

    // Reset throttle
    CurrentThrottle = 0.f;
}
