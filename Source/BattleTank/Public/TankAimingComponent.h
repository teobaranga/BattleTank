// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetBarrel(UStaticMeshComponent* Barrel);

	void AimAt(FVector Location, float LaunchSpeed);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UStaticMeshComponent* Barrel = nullptr;

	USceneComponent* BarrelRotator = nullptr;

	// Maximum movement speed of the barrel, in degrees per second
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 20.f;

	// Minimum barrel elevation in degrees
	UPROPERTY(EditAnywhere)
	float MinElevation = 0.f;

	// Maximum barrel elevation in degrees
	UPROPERTY(EditAnywhere)
	float MaxElevation = 35.f;

	void MoveBarrel(const FVector& AimDirection);

	void ElevateBarrel(float DegreesPerSecond);
	
};
