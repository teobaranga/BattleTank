// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDeath);

UCLASS()
class BATTLETANK_API ATankPawn : public APawn
{
    GENERATED_BODY()

public:
    // Return current health as a percentage of starting health, between 0 and 1
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent() const;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Called by the engine when actor damage is dealt
    float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    FTankDeath OnDeath;

private:
    // Sets default values for this pawn's properties
    ATankPawn();

    void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    float InitialHealth = 100.f;

    UPROPERTY(VisibleAnywhere, Category = "Setup")
    float Health;
};
