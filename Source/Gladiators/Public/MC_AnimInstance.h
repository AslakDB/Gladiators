// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MC_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORS_API UMC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;

public:
	

	UFUNCTION(BlueprintCallable)
		void ResetAttack();
	UFUNCTION(BlueprintCallable)
		void GetIsAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		bool SwordIsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		bool SpearIsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		bool AxeIsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class AMySweetBabyBoi* Boi;
	
};
