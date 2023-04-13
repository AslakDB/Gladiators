// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUserWidget.h"
#include "BossWidget.h"
// Sets default values
ABasicTest::ABasicTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	//BossWidget = CreateWidget<UBossWidget>(GetWorld(), TBossWidget);
	MaxHealth = 100;
	Health = MaxHealth;
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
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Nullpt error"));
		}

		BossWidget = CreateWidget<UBossWidget>(World, TBossWidget);
	}
}

// Called every frame
void ABasicTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Widget)
	{
		Widget->SetPlayerHealth(Health, MaxHealth);
	}

	if (BossWidget)
	{
		BossWidget->AddToViewport(999);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(7, 60.f, FColor::Red, FString("Nullpt error for bossWidget"));
	}
	TestFunction();
}

// Called to bind functionality to input
void ABasicTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicTest::TestFunction()
{
	Health--;
}

