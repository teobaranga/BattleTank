// Copyright (c) 2018 Teo Baranga

#include "TankPawn.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ATankPawn::ATankPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming"));
    TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement"));
}

void ATankPawn::Initialize(UStaticMeshComponent* Barrel)
{
    this->Barrel = Barrel;
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Input is done in blueprints
}

void ATankPawn::AimAt(FVector HitLocation)
{
    if (!TankAimingComponent->IsRegistered())
    {
        return;
    }

    TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATankPawn::Fire()
{
    if (IsPendingKillPending())
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot fire, tank %s destroyed"), *GetName());
        return;
    }

    //UE_LOG(LogTemp, Warning, TEXT("Fire!"));

    bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTime;

    if (isReloaded)
    {
        FTransform ProjectileSpawnLocation = Barrel->GetSocketTransform(FName("Projectile"));
        AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(Projectile, ProjectileSpawnLocation);
        ProjectileInstance->Launch(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
    }
}
