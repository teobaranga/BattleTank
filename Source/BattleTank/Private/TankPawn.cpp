// Copyright (c) 2018 Teo Baranga

#include "TankPawn.h"
#include "../Public/TankPawn.h"


// Sets default values
ATankPawn::ATankPawn()
{
    PrimaryActorTick.bCanEverTick = false;
}

float ATankPawn::GetHealthPercent() const
{
    return Health / InitialHealth;
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Input is done in blueprints
}

float ATankPawn::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    UE_LOG(LogTemp, Warning, TEXT("Actual Damage before: %f"), ActualDamage);

    ActualDamage = FMath::Clamp(ActualDamage, 0.f, Health);

    UE_LOG(LogTemp, Warning, TEXT("Actual Damage after: %f"), ActualDamage);

    Health -= ActualDamage;

    if (Health <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tank %s is ded"), *GetName());
    }

    return ActualDamage;
}
