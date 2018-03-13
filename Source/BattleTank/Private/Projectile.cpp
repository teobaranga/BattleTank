// Copyright (c) 2018 Teo Baranga

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
    CollisionMesh->SetNotifyRigidBodyCollision(true);
    CollisionMesh->SetVisibility(false);
    SetRootComponent(CollisionMesh);

    LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Particle System"));
    LaunchBlast->SetupAttachment(RootComponent);

    ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
    ImpactBlast->SetupAttachment(RootComponent);
    ImpactBlast->bAutoActivate = false;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
    ProjectileMovementComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    LaunchBlast->OnSystemFinished.AddDynamic(this, &AProjectile::OnLaunchBlastFinished);
    ImpactBlast->OnSystemFinished.AddDynamic(this, &AProjectile::OnImpactBlastFinished);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    LaunchBlast->Deactivate();
    ImpactBlast->Activate();
    CollisionMesh->DestroyComponent();
}

void AProjectile::OnLaunchBlastFinished(UParticleSystemComponent* PSystem)
{
    LaunchBlastFinished = true;
    if (ImpactBlastFinished)
    {
        Destroy();
    }
}

void AProjectile::OnImpactBlastFinished(UParticleSystemComponent* PSystem)
{
    ImpactBlastFinished = true;
    if (LaunchBlastFinished)
    {
        Destroy();
    }
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectile::Launch(float Speed)
{
    ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::RightVector * Speed);
    ProjectileMovementComponent->Activate();
}

