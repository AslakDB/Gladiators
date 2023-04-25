// Fill out your copyright notice in the Description page of Project Settings.


#include "MC_AnimInstance.h"

#include "Gladiators/MySweetBabyBoi.h"

void UMC_AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn&&Boi == nullptr)
		{
			Boi = Cast<AMySweetBabyBoi>(Pawn);

		}
	}
	AxeIsAttack = false;
	SpearIsAttack = false;
	SwordIsAttack = false;
}

void UMC_AnimInstance::ResetAttack()
{
	if (Boi)
	{
		Boi->ResetAttack();
	}
}

void UMC_AnimInstance::GetIsAttack()
{
	if(Boi)
	{
		SwordIsAttack = Boi->GetIsAttack();
	}
}
