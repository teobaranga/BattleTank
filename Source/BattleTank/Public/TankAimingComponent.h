// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class AProjectile;

UENUM()
enum class EFiringState : uint8
{
    OutOfAmmo,
    Reloading,
    Aiming,
    Locked
};

/**
 * Aiming component for a tank-like actor.
 *
 * Note: the associated actor must have its forward vector pointing in the Y axis
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UTankAimingComponent();

    void BeginPlay() override;

    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    void AimAt(FVector Location);

    UFUNCTION(BlueprintCallable)
    void Fire();

    EFiringState GetFiringState() const;

protected:
    /** The current firing state */
    UPROPERTY(BlueprintReadOnly)
    EFiringState FiringState = EFiringState::Reloading;

    /**
     * Initialize the aiming component.
     */
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialize(UStaticMeshComponent* Turret, UStaticMeshComponent* Barrel, UStaticMeshComponent* BarrelRotator);

    // Current ammo
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
    int32 Ammo = 3;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    TSubclassOf<AProjectile> Projectile = nullptr;

    // Muzzle velocity
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 4000; // 4000 cm/s

    UPROPERTY(EditDefaultsOnly, Category="Firing")
    double ReloadTime = 0.1;

    // Maximum elevation speed of the barrel, in degrees per second
    UPROPERTY(EditDefaultsOnly)
    float MaxElevationSpeed = 5.f;

    // Maximum rotation speed of the turret, in degrees per second
    UPROPERTY(EditDefaultsOnly)
    float MaxRotationSpeed = 15.f;

    // Minimum barrel elevation in degrees
    UPROPERTY(EditDefaultsOnly)
    float MinElevation = 0.f;

    // Maximum barrel elevation in degrees
    UPROPERTY(EditDefaultsOnly)
    float MaxElevation = 35.f;

    // Socket name of the barrel muzzle - used for projectile calculations
    static const FName BarrelMuzzleSocketName;

    // Socket name of the barrel projectile - used for projectile spawning
    static const FName BarrelProjectileSocketName;

    // Turret component - rotates around the Z axis
    UStaticMeshComponent* Turret = nullptr;

    // The barrel component - rotates around the Y axis
    UStaticMeshComponent* Barrel = nullptr;

    /**
    * The barrel joint - this is needed when the elevation axis of rotation is
    * different from the barrel's origin
    */
    USceneComponent* BarrelRotator = nullptr;

    // The actors to be ignored when aiming. By default, this is simply the Tank pawn.
    TArray<AActor*> AimActorsToIgnore;

    double LastFireTime = 0;

    bool canFire = false;

    bool isAiming = false;

    void MoveBarrel(const FVector& AimDirection);

    /**
     * Move the barrel up or down
     *
     * @param RelativeSpeed: -1 is max downward speed, +1 is max up movement
     */
    void ElevateBarrel(float RelativeSpeed);

    // Rotate the turret around its Z-axis
    void RotateBarrel(float RelativeSpeed);
};
