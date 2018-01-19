// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
    Super::BeginPlay();

    /// Find the turret component
    TArray<UActorComponent*> Turrets = GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Turret"));
    if (Turrets.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Turret not found"));
        DestroyComponent();
        return;
    }
    Turret = Cast<UStaticMeshComponent>(Turrets[0]);

    /// Find the barrel and its rotator
    TArray<UActorComponent*> Barrels = GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), FName("Barrel"));
    for (UActorComponent* Barrel : Barrels)
    {
        if (Barrel->ComponentTags.Num() == 1)
        {
            this->Barrel = Cast<UStaticMeshComponent>(Barrel);
        }
        else if (Barrel->ComponentHasTag(FName("Rotator")))
        {
            BarrelRotator = Cast<USceneComponent>(Barrel);
        }
    }
    if (!Barrel)
    {
        UE_LOG(LogTemp, Error, TEXT("Barrel not found"));
        DestroyComponent();
        return;
    }
    if (!BarrelRotator)
    {
        UE_LOG(LogTemp, Error, TEXT("Barrel rotator not found"));
        DestroyComponent();
        return;
    }
}

void UTankAimingComponent::MoveBarrel(const FVector& AimDirection)
{
    FRotator AimRotator = AimDirection.ToOrientationRotator();

    FRotator BarrelRotator = Barrel->GetRightVector().ToOrientationRotator();
    FRotator DeltaRotator = AimRotator - BarrelRotator;
    //UE_LOG(LogTemp, Warning, TEXT("Aim rotator %s"), *AimRotator.ToCompactString());
    //UE_LOG(LogTemp, Warning, TEXT("Barrel rotator %s"), *BarrelRotator.ToCompactString());
    //UE_LOG(LogTemp, Warning, TEXT("Delta vector %s"), *DeltaRotator.ToCompactString());

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

void UTankAimingComponent::AimAt(FVector Location, float LaunchSpeed)
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
