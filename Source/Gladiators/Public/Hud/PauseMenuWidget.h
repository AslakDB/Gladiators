// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */

class UButton;
UCLASS()
class GLADIATORS_API UPauseMenuWidget : public UUserWidget
{
public:
	GENERATED_BODY()
	
		UPROPERTY(VisibleAnywhere)
		bool Paused;
	UPROPERTY(EditAnywhere,meta = (BindWidget))
		UButton* Resume;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Options;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Quit;
	UFUNCTION()
		void PauseMenuManager();
	
};
