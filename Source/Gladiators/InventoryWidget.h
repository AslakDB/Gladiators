// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class UImage;
UCLASS()
class GLADIATORS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

		UPROPERTY(VisibleAnywhere)
		int InventoryCount;

	
};
