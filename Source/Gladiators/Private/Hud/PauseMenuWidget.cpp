// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"




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
	/*APlayerController* PlayerController = Cast<APlayerController>(Controller);
	PlayerController->SetShowMouseCursor(true);*/
	
		Paused = false;
		UGameplayStatics::SetGamePaused(this, false);
	
}

void UPauseMenuWidget::QuitGameBoy()
{
	UGameplayStatics::OpenLevel(this, "MainMenu");
}
