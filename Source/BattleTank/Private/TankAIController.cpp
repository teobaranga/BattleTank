// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

#include "Engine/World.h"

#include "TankPlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ATankPawn* Tank = GetPawn())
	{
		if (ATankPawn* PlayerTank = GetPlayerTank())
		{
			Tank->AimAt(PlayerTank->GetActorLocation());
		}
	}
}

ATankPawn* ATankAIController::GetPawn() const
{
	return Cast<ATankPawn>(AController::GetPawn());
}

ATankPawn * ATankAIController::GetPlayerTank() const
{
	if (ATankPlayerController* tankPlayerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		return tankPlayerController->GetPawn();
	}

	UE_LOG(LogTemp, Error, TEXT("Unexpected player controller. Player Tank could not be found."));
	return nullptr;
}
