// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWidget.h"
#include "Components/ProgressBar.h"

void UBossWidget::SetBossHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
