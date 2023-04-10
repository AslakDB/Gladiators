// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest.h"

// Sets default values
ABasicTest::ABasicTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

