// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Axe.h"
#include "Components/SphereComponent.h"

// Sets default values
AAxe::AAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AxeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(AxeCollider);
	AxeCollider->bHiddenInGame = false;
	AxeCollider->InitSphereRadius(50.f);
	AxeCollider->OnComponentBeginOverlap.AddDynamic(this, &AAxe::AxeOnOverlapBegin);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AAxe::BeginPlay()
{
	Super::BeginPlay();

	AxeCollider->OnComponentBeginOverlap.AddDynamic(this, &AAxe::AxeOnOverlapBegin);
}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAxe::AxeOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AAxe::Pickup()
{
	Destroy();
}

