// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	void SetHealthBarPercent(float Percent);

	UPROPERTY()
		class UHealthBar* HealthBarWidget;
};
