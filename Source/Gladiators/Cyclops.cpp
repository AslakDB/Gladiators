// Fill out your copyright notice in the Description page of Project Settings.


#include "Cyclops.h"

#include "GameManager.h"


// Sets default values
ACyclops::ACyclops()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameManager = CreateDefaultSubobject<AGameManager>(TEXT("GameManager"));
}

// Called when the game starts or when spawned
void ACyclops::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACyclops::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GameManager)
		{
	
		
		GEngine->AddOnScreenDebugMessage(6, 60.f, FColor::Green, FString("GameManager is real"));
		GameManager->WaveNumb = 2;
			if (GameManager->WaveNumb == 2)
			{
				//Code for the cyclops to start moving
				GEngine->AddOnScreenDebugMessage(8, 60.f, FColor::Cyan, FString("Wave 2 start"));
			}
			
		}
	else
	{
		GEngine->AddOnScreenDebugMessage(6, 60.f, FColor::Red, FString("GameManager is not real"));
	}
}

