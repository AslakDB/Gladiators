// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
class UProgressBar;
UCLASS()
class GLADIATORS_API UEnemyHealthBar : public UWidgetComponent
{
	GENERATED_BODY()


		UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* EnemyHealthBar;

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void SetEnemyHealth(float CurrentHealth, float MaxHealth);
};
