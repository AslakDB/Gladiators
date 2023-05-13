// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* EnemyHealthBar;

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void SetEnemyHealth(float CurrentHealth, float MaxHealth);
	
};
