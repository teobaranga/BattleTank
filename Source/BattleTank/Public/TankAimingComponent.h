// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UTankAimingComponent();

    void AimAt(FVector Location, float LaunchSpeed);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    UStaticMeshComponent* Turret = nullptr;

    UStaticMeshComponent* Barrel = nullptr;

    USceneComponent* BarrelRotator = nullptr;

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

    void MoveBarrel(const FVector& AimDirection);

    // Move the barrel up or down
    // -1 is max downward speed, +1 is max up movement
    void ElevateBarrel(float RelativeSpeed);
    
    // Rotate the turret around its Z-axis
    void RotateBarrel(float RelativeSpeed);
};
