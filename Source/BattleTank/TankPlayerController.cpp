// Copyright (c) 2018 Teo Baranga

#include "TankPlayerController.h"

#include "Engine/World.h"

#include "Public/TankPawn.h"
#include "Public/TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    UTankAimingComponent* TankAimingComponent = GetTankPawn()->FindComponentByClass<UTankAimingComponent>();
    if (TankAimingComponent)
    {
        FoundAimingComponent(TankAimingComponent);
    }
}

void ATankPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    AimTowardsCrosshair();
}

ATankPawn* ATankPlayerController::GetTankPawn() const
{
    return Cast<ATankPawn>(AController::GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
    if (!ensure(GetTankPawn()))
    {
        return;
    }

    FVector HitLocation;
    if (GetSightRayHitLocation(HitLocation))
    {
        GetTankPawn()->AimAt(HitLocation);
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
