// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "HUD")
		void SetBossHealth(float CurrentHealth, float MaxHealth);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* HealthBar;
};
