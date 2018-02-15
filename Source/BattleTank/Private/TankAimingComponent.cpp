// Copyright (c) 2018 Teo Baranga

#include "TankAimingComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
}

void UTankAimingComponent::BeginPlay()
{
    Super::BeginPlay();
    LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialize(UStaticMeshComponent* Turret, UStaticMeshComponent* Barrel, UStaticMeshComponent* BarrelRotator)
{
    this->Turret = Turret;
    this->Barrel = Barrel;
    this->BarrelRotator = BarrelRotator;
    if (!Projectile)
    {
        UE_LOG(LogTemp, Error, TEXT("Projectile missing, %s will not be able to fire"), *GetName());
        canFire = false;
    }
    else
    {
        canFire = true;
    }
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    switch (FiringState)
    {
    case EFiringState::Reloading:
        // Move to the aiming state if the reloading time has elapsed
        if ((FPlatformTime::Seconds() - LastFireTime) > ReloadTime)
        {
            FiringState = EFiringState::Aiming;
        }
        break;
    case EFiringState::Aiming:
        // if barrel is not moving, switch to locked
        if (!isAiming)
        {
            FiringState = EFiringState::Locked;
        }
        break;
    case EFiringState::Locked:
        // if barrel is moving, switch to aiming
        if (isAiming)
        {
            FiringState = EFiringState::Aiming;
        }
        break;
    default:
        break;
    }
    
}

void UTankAimingComponent::MoveBarrel(const FVector& AimDirection)
{
    FRotator AimRotator = AimDirection.ToOrientationRotator();

    FRotator BarrelRotator = Barrel->GetRightVector().ToOrientationRotator();
    FRotator DeltaRotator = AimRotator - BarrelRotator;

    ElevateBarrel(DeltaRotator.Pitch);

    /// Make sure that the turret picks the smallest rotation when turning towards the aim point
    if (DeltaRotator.Yaw < -180.f)
    {
        DeltaRotator.Yaw += 360.f;
    }
    else if (DeltaRotator.Yaw > 180.f)
    {
        DeltaRotator.Yaw -= 360.f;
    }

    RotateBarrel(DeltaRotator.Yaw);

    isAiming = !(FMath::IsNearlyZero(DeltaRotator.Pitch, 0.5f) && FMath::IsNearlyZero(DeltaRotator.Yaw, 0.5f));
}

void UTankAimingComponent::ElevateBarrel(float RelativeSpeed)
{
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);

    float ElevationDelta = RelativeSpeed * MaxElevationSpeed * GetWorld()->GetDeltaSeconds();
    FRotator Rotation = BarrelRotator->RelativeRotation;
    Rotation.Pitch = FMath::ClampAngle(Rotation.Pitch + ElevationDelta, MinElevation, MaxElevation);

    BarrelRotator->SetRelativeRotation(Rotation);
}

void UTankAimingComponent::RotateBarrel(float RelativeSpeed)
{
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);

    float RotationDelta = RelativeSpeed * MaxRotationSpeed * GetWorld()->GetDeltaSeconds();
    FRotator Rotation = Turret->RelativeRotation;
    Rotation.Yaw = Rotation.Yaw + RotationDelta;

    Turret->SetRelativeRotation(Rotation);
}

void UTankAimingComponent::AimAt(FVector Location)
{
    FVector TossVelocity;
    if (UGameplayStatics::SuggestProjectileVelocity(this, TossVelocity, Barrel->GetSocketLocation(FName("Muzzle")), Location, LaunchSpeed,
        false, 0, 0, ESuggestProjVelocityTraceOption::OnlyTraceWhileAscending, FCollisionResponseParams::DefaultResponseParam, TArray<AActor*>(), false)
        )
    {
        FVector AimDirection = TossVelocity.GetSafeNormal();
        //UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *GetOwner()->GetName(), *AimDirection.ToCompactString());
        MoveBarrel(AimDirection);
    }
}

void UTankAimingComponent::Fire()
{
    if (!IsRegistered())
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot fire, tank aiming component on %s not available"), *GetOwner()->GetName());
        return;
    }

    if (!canFire)
    {
        return;
    }

    //UE_LOG(LogTemp, Warning, TEXT("Fire!"));

    if (FiringState != EFiringState::Reloading)
    {
        FTransform ProjectileSpawnLocation = Barrel->GetSocketTransform(FName("Projectile"));
        AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(Projectile, ProjectileSpawnLocation);
        ProjectileInstance->Launch(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
        FiringState = EFiringState::Reloading;
    }
}
