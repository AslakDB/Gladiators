// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/EnemyHealthWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHealthWidget::SetEnemyHealth(float CurrentHealth, float MaxHealth)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
