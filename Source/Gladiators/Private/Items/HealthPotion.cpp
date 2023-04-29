// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthPotion.h"
#include "Components/SphereComponent.h"

// Sets default values
AHealthPotion::AHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PotionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(PotionCollider);
	PotionCollider->bHiddenInGame = false;
	PotionCollider->InitSphereRadius(50.f);
	PotionCollider->OnComponentBeginOverlap.AddDynamic(this, &AHealthPotion::PotionOnOverlapBegin);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
}


// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	PotionCollider->OnComponentBeginOverlap.AddDynamic(this, &AHealthPotion::PotionOnOverlapBegin);
}

// Called every frame
void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPotion::PotionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AHealthPotion::Pickup()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	Destroy();
}

