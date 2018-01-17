// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include "TankAimingComponent.h"


// Sets default values
ATankPawn::ATankPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming"));
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
    Super::BeginPlay();
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::AimAt(FVector HitLocation)
{
    TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}
