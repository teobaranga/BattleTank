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
};
