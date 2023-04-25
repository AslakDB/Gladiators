// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API AGameManager : public AGameModeBase
{
	GENERATED_BODY()
public:
		UPROPERTY()
		bool Paused;

	UPROPERTY()
		int WaveNum;


};
