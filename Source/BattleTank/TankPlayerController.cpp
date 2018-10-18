// Copyright (c) 2018 Teo Baranga

#include "TankPlayerController.h"
#include "Public/TankAimingComponent.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

    TankCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
    TankCollisionQueryParams.AddIgnoredActor(GetPawn());
}

void ATankPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    AimTowardsCrosshair();
}

void ATankPlayerController::BeginPlayingState()
{
    Super::BeginPlayingState();
    TankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (TankAimingComponent)
    {
        OnPossess(TankAimingComponent);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Tank Aiming Component not found"));
    }
}

void ATankPlayerController::EndPlayingState()
{
    Super::EndPlayingState();
    TankAimingComponent = nullptr;
    OnUnPossess();
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
        //DrawDebugSphere(GetWorld(), WorldLocation, 20.f, 10, FColor::Red, false, 1.f, 0, 10.f);

        /// Line-trace along that look direction, and see what we hit (up to a max range)
        FHitResult HitResult;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * LineTraceRange,
                                                ECollisionChannel::ECC_Visibility, TankCollisionQueryParams))
        {
            OutHitLocation = HitResult.Location;
            //DrawDebugSphere(GetWorld(), OutHitLocation, 20.f, 10, FColor::Blue, false, 1.f, 0, 10.f);
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Line trace failed at %s in dir %s"), *WorldLocation.ToCompactString(), *WorldDirection.ToCompactString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to deproject screen coords (%.2f, %.2f) to world"), ScreenLocation.X, ScreenLocation.Y);
    }

    return false;
}
