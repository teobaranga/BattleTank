// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"


UCLASS()
class BATTLETANK_API ATankPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ATankPawn();

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    float InitialHealth = 100.f;

    UPROPERTY(VisibleAnywhere, Category = "Setup")
    float Health = InitialHealth;

    float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
