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

		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere)
		int InventoryCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* EmptyImage1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* EmptyImage2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* EmptyImage3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* FullImage1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* FullImage2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* FullImage3;
	
	UFUNCTION(BlueprintCallable)
		void AddToInventory();

	UFUNCTION(BlueprintCallable)
		void RemoveFromInventory();

	UFUNCTION(BlueprintCallable)
		void ManageInventory();
};
