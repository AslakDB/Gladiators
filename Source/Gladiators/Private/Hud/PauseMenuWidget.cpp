// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Gladiators/MySweetBabyBoi.h"
#include "Kismet/GameplayStatics.h"


void UPauseMenuWidget::NativeConstruct()
{
	
	Resume->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumePlayBitch);
	Quit->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitGameBoy);
}

void UPauseMenuWidget::PauseMenuManager()
{
	
	
}

void UPauseMenuWidget::ResumePlayBitch()
{
	ACharacter* Player;
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APlayerController* PlayerController = Cast<APlayerController>(Player->Controller);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, true);
		Paused = false;
		UGameplayStatics::SetGamePaused(this, false);
	
}

void UPauseMenuWidget::QuitGameBoy()
{
	UGameplayStatics::OpenLevel(this, "MainMenu");
}
