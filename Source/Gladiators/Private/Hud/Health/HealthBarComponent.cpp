// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/Health/HealthBarComponent.h"
#include "Hud/Health/HealthBar.h"
#include "Components/ProgressBar.h"
void UHealthBarComponent::SetHealthBarPercent(float Percent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	else
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
