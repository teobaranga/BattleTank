// Copyright (c) 2018 Teo Baranga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AProjectile();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* CollisionMesh = nullptr;
    
    UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* LaunchBlast = nullptr;

    UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* ImpactBlast = nullptr;

    UPROPERTY(VisibleAnywhere)
    URadialForceComponent* ExplosionForce = nullptr;


    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnLaunchBlastFinished(UParticleSystemComponent* PSystem);

    UFUNCTION()
    void OnImpactBlastFinished(UParticleSystemComponent* PSystem);

public:	
    UPROPERTY(EditDefaultsOnly)
    float Damage = 20.f;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Launch this projectile
    void Launch(float Speed);

private:
    bool LaunchBlastFinished = false;

    bool ImpactBlastFinished = false;
    
};
