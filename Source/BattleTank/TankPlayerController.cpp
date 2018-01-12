// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	AimTowardsCrosshair();
}

ATankPawn* ATankPlayerController::GetPawn() const
{
	return Cast<ATankPawn>(AController::GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn())
	{
		return;
	}

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{

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
		FHitResult OutHitLocation;
		if (GetWorld()->LineTraceSingleByChannel(OutHitLocation, WorldLocation, WorldLocation + WorldDirection * LineTraceRange,
												ECollisionChannel::ECC_Visibility))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit in direction %s"), *OutHitLocation.ImpactPoint.ToCompactString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No hit"));
		}
	}

	return false;
}
