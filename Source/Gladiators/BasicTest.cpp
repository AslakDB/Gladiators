// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUserWidget.h"
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
	if (UWorld* World = GetWorld())
	{
		Widget = CreateWidget<UPlayerUserWidget>(World, TWidget);
		if (Widget)
		{
			Widget->AddToViewport(999);
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("No Nullpt"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("No Nullpt error"));
		}
	}
}

// Called every frame
void ABasicTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Widget)
	{
		Widget->SetPlayerHealth(100, 100);
	}
}

// Called to bind functionality to input
void ABasicTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

