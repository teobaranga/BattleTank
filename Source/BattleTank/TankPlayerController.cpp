// Copyright (c) 2018 Teo Baranga

#include "TankPlayerController.h"
#include "Public/TankAimingComponent.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    TankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (TankAimingComponent)
    {
        FoundAimingComponent(TankAimingComponent);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Tank Aiming Component not found"));
    }
}

void ATankPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
    if (!TankAimingComponent)
    {
        return;
    }

    FVector HitLocation;
    if (GetSightRayHitLocation(HitLocation))
    {
        TankAimingComponent->AimAt(HitLocation);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No hit"));
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);

    /// Find the crosshair position
    FVector2D ScreenLocation(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

    /// "De-project" the screen position of the crosshair to a world direction
    FVector WorldLocation, WorldDirection;
    if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection))
    {
        /// Line-trace along that look direction, and see what we hit (up to a max range)
        FHitResult HitResult;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * LineTraceRange,
                                                ECollisionChannel::ECC_Visibility))
        {
            OutHitLocation = HitResult.Location;
            return true;
        }
    }

    return false;
}
