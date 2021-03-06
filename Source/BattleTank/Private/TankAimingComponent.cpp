// Copyright (c) 2018 Teo Baranga

#include "TankAimingComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

const FName UTankAimingComponent::BarrelMuzzleSocketName = FName("Muzzle");
const FName UTankAimingComponent::BarrelProjectileSocketName = FName("Projectile");

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
    this->AimActorsToIgnore = { GetOwner() };
    if (!Projectile)
    {
        UE_LOG(LogTemp, Error, TEXT("%s - Projectile missing, firing disabled"), *GetOwner()->GetName());
        canFire = false;
    }
    else
    {
        canFire = true;
    }
    if (!this->Barrel->DoesSocketExist(BarrelMuzzleSocketName))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s - Missing barrel muzzle socket"), *GetOwner()->GetName());
    }
    if (!this->Barrel->DoesSocketExist(BarrelProjectileSocketName))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s - Missing barrel projectile socket"), *GetOwner()->GetName());
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

    // Make sure that the turret picks the smallest rotation when turning towards the aim point
    if (FMath::Abs(DeltaRotator.Yaw) < 180.f)
    {
        RotateBarrel(DeltaRotator.Yaw);
    }
    else
    {
        RotateBarrel(-DeltaRotator.Yaw);
    }

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
    if (UGameplayStatics::SuggestProjectileVelocity(this, TossVelocity, Barrel->GetSocketLocation(BarrelMuzzleSocketName), Location, LaunchSpeed,
        false, 0, 0, ESuggestProjVelocityTraceOption::OnlyTraceWhileAscending, FCollisionResponseParams::DefaultResponseParam, AimActorsToIgnore, false)
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

    if (!canFire || Ammo <= 0)
    {
        return;
    }

    //UE_LOG(LogTemp, Warning, TEXT("Fire!"));

    if (FiringState == EFiringState::Aiming || FiringState == EFiringState::Locked)
    {
        FTransform ProjectileSpawnLocation = Barrel->GetSocketTransform(BarrelProjectileSocketName);
        AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(Projectile, ProjectileSpawnLocation);
        ProjectileInstance->Launch(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();

        if (--Ammo <= 0)
        {
            FiringState = EFiringState::OutOfAmmo;
        }
        else
        {
            FiringState = EFiringState::Reloading;
        }
    }
}

EFiringState UTankAimingComponent::GetFiringState() const
{
    return FiringState;
}
