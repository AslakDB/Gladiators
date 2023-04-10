// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUserWidget.h"

#include "Components/ProgressBar.h"

void UPlayerUserWidget::SetPlayerHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
