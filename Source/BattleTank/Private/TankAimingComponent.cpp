// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> Barrels = GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), FName("Barrel"));
	TArray<UActorComponent*> Rotators = GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), FName("Rotator"));

	for (UActorComponent* Barrel : Barrels)
	{
		for (UActorComponent* Rotator : Rotators)
		{
			if (Barrel == Rotator)
			{
				BarrelRotator = Cast<USceneComponent>(Barrel);
				UE_LOG(LogTemp, Warning, TEXT("Found the barrel rotator"));
				break;
			}
		}
	}
}

void UTankAimingComponent::SetBarrel(UStaticMeshComponent * Barrel)
{
	this->Barrel = Barrel;
}

void UTankAimingComponent::MoveBarrel(const FVector& AimDirection)
{
	FRotator BarrelRotator = Barrel->GetRightVector().ToOrientationRotator();
	FRotator AimRotator = AimDirection.ToOrientationRotator();
	UE_LOG(LogTemp, Warning, TEXT("Barrel vector %s"), *AimRotator.ToCompactString());
}

void UTankAimingComponent::ElevateBarrel(float DegreesPerSecond)
{
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankAimingComponent::AimAt(FVector Location, float LaunchSpeed)
{
	if (!Barrel)
	{
		return;
	}

	FVector TossVelocity;
	if (UGameplayStatics::SuggestProjectileVelocity(this, TossVelocity, Barrel->GetSocketLocation(FName("Muzzle")), Location, LaunchSpeed))
	{
		FVector AimDirection = TossVelocity.GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *GetOwner()->GetName(), *AimDirection.ToCompactString());
		MoveBarrel(AimDirection);
	}
}
