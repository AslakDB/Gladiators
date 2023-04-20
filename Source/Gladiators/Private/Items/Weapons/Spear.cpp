// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Spear.h"

// Sets default values
ASpear::ASpear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpear::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

