// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::SetEnemyHealth(float CurrentHealth, float MaxHealth)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetPercent(CurrentHealth/ MaxHealth);
	}
}
