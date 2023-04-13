// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		int InventoryCount;

	
	UFUNCTION(BlueprintCallable)
		void AddToInventory();

	UFUNCTION(BlueprintCallable)
		void RemoveFromInventory();

	UFUNCTION(BlueprintCallable)
		void ManageInventory();
};
