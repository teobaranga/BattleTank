// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UTankAimingComponent;
class UTankMovementComponent;
class AProjectile;

UCLASS()
class BATTLETANK_API ATankPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ATankPawn();

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void AimAt(FVector HitLocation);

    UFUNCTION(BlueprintCallable)
    void Fire();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTankAimingComponent* TankAimingComponent = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTankMovementComponent* TankMovementComponent = nullptr;

private:
    UPROPERTY(EditDefaultsOnly, Category="Projectile")
    TSubclassOf<AProjectile> Projectile = nullptr;

    // Muzzle velocity
    UPROPERTY(EditDefaultsOnly, Category="Projectile")
    float LaunchSpeed = 150000; // 1500 m/s

    UStaticMeshComponent* Barrel = nullptr;
    
    double ReloadTime = 2;

    double LastFireTime = 0;
};
