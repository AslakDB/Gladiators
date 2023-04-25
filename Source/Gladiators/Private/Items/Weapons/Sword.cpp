// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Sword.h"
#include "Components/SphereComponent.h"


// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->bHiddenInGame = false;
	Collider->InitSphereRadius(100.f);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnOverlapBegin);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnOverlapBegin);
	
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ASword::Pickup()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	Destroy();
}

