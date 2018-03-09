// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AProjectile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* CollisionMesh = nullptr;

    UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* LaunchBlast = nullptr;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Launch this projectile
    void Launch(float Speed);
    
};
