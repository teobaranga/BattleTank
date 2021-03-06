// Copyright (c) 2018 Teo Baranga

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "TankPawn.h" // So we can implement OnDeath()
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    if (APawn* Tank = GetPawn())
    {
        TankAimingComponent = Tank->FindComponentByClass<UTankAimingComponent>();
    }
    PlayerTank = GetPlayerTank();

    if (!TankAimingComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Tank Aiming Component not found"));
    }
    if (!PlayerTank)
    {
        UE_LOG(LogTemp, Error, TEXT("Player Tank not found"));
    }
}

void ATankAIController::SetPawn(APawn * InPawn)
{
    Super::SetPawn(InPawn);

    ATankPawn* PossessedTank;
    if (InPawn && (PossessedTank = Cast<ATankPawn>(InPawn)) != 0)
    {
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
    }
}

void ATankAIController::OnTankDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("[%s] Tank pawn is dead"), *(this->GetName()));

    if (APawn* Tank = GetPawn())
    {
        Tank->DetachFromControllerPendingDestroy();
    }
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TankAimingComponent || !TankAimingComponent->IsRegistered() || !PlayerTank)
    {
        return;
    }

    // Move towards player
    MoveToActor(PlayerTank, AcceptanceRadius, true, true, false);

    // Aim at player
    TankAimingComponent->AimAt(PlayerTank->GetActorLocation());

    // Fire if ready
    EFiringState FiringState = TankAimingComponent->GetFiringState();
    if (FiringState == EFiringState::Locked)
    {
        TankAimingComponent->Fire();
    }
}

APawn* ATankAIController::GetPlayerTank() const
{
    if (APlayerController* tankPlayerController = GetWorld()->GetFirstPlayerController())
    {
        return tankPlayerController->GetPawn();
    }

    UE_LOG(LogTemp, Error, TEXT("Unexpected player controller. Player Tank could not be found."));
    return nullptr;
}
