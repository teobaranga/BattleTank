// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/TankPawn.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	// Get the Tank Pawn controlled by this AI
	ATankPawn* GetPawn() const;

	// Get the Tank Pawn controlled by the player
	ATankPawn* GetPlayerTank() const;
};
