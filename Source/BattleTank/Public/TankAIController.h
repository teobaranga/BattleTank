// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Get the Tank Pawn controlled by the player
    APawn* GetPlayerTank() const;

private:
    /* The aiming component of the owned tank */
    UTankAimingComponent* TankAimingComponent = nullptr;

    /* The tank owned by the player */
    AActor* PlayerTank = nullptr;

    // How close the AI can get to the player
    UPROPERTY(EditDefaultsOnly)
    float AcceptanceRadius = 3000.f;

    virtual void SetPawn(APawn* InPawn) override;

    UFUNCTION()
    void OnTankDeath();
};
