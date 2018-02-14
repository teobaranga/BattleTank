// Copyright (c) 2018 Teo Baranga

#include "TankPawn.h"


// Sets default values
ATankPawn::ATankPawn()
{
    PrimaryActorTick.bCanEverTick = false;
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Input is done in blueprints
}
