// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Spear.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpear::ASpear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpearCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(SpearCollider);
	SpearCollider->bHiddenInGame = false;
	SpearCollider->InitSphereRadius(50.f);
	SpearCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpear::SpearOnOverlapBegin);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASpear::BeginPlay()
{
	Super::BeginPlay();

	SpearCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpear::SpearOnOverlapBegin);

}

// Called every frame
void ASpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpear::SpearOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ASpear::Pickup()
{
	//SetActorHiddenInGame(true);
	Destroy();
}

