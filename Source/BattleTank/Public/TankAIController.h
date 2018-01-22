// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATankPawn;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    
public:
    virtual void Tick(float DeltaTime) override;

    // Get the Tank Pawn controlled by this AI
    ATankPawn* GetPawn() const;

    // Get the Tank Pawn controlled by the player
    ATankPawn* GetPlayerTank() const;

private:
    // How close the AI can get to the player
    float AcceptanceRadius = 3000.f;
};
