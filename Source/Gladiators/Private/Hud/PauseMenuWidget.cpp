// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UPauseMenuWidget::PauseMenuManager()
{
	if (Resume->IsPressed())
	{
		Paused = false;
	}
	if (Quit->IsPressed())
	{
	GEngine->AddOnScreenDebugMessage(1, 7, FColor::Black, TEXT("Quit has been pressed"));
		UGameplayStatics::OpenLevel(this,"MainMenu");
	}
}
