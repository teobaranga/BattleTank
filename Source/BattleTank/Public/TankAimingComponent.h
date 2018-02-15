// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class AProjectile;

UENUM()
enum class EFiringState : uint8
{
    Reloading,
    Aiming,
    Locked
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

protected:
    UPROPERTY(BlueprintReadOnly)
    EFiringState FiringState = EFiringState::Reloading;

    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialize(UStaticMeshComponent* Turret, UStaticMeshComponent* Barrel, UStaticMeshComponent* BarrelRotator);

private:
    UStaticMeshComponent* Turret = nullptr;

    UStaticMeshComponent* Barrel = nullptr;

    USceneComponent* BarrelRotator = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<AProjectile> Projectile = nullptr;

    // Muzzle velocity
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float LaunchSpeed = 4000; // 4000 cm/s

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

    double ReloadTime = 2;

    double LastFireTime = 0;

    bool canFire = false;

    bool isAiming = false;

    void MoveBarrel(const FVector& AimDirection);

    // Move the barrel up or down
    // -1 is max downward speed, +1 is max up movement
    void ElevateBarrel(float RelativeSpeed);
    
    // Rotate the turret around its Z-axis
    void RotateBarrel(float RelativeSpeed);
};
