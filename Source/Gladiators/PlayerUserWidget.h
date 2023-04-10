// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API UPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable, Category = "HUD")
		void SetPlayerHealth(float CurrentHealth, float MaxHealth);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* HealthBar;
};
