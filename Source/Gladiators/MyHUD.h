// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
		/*Get the player health to cast to Hud */
		UFUNCTION(BlueprintCallable, Category = "HUD")
		void GetPlayerHealth(float Health, float MaxHealth);


	/*Get the score to cast to HUD*/
	UFUNCTION(BlueprintCallable, Category = "HUD")
		void GetScore();

	/*Gets the UI descriptions for items to the HUD */
	UFUNCTION(BlueprintCallable, Category = "HUD")
		void GetItemDescription();

	
};
