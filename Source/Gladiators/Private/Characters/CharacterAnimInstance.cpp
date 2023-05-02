// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterAnimInstance.h"
#include "Gladiators/MySweetBabyBoi.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MySweetBabyBoi = Cast<AMySweetBabyBoi>(TryGetPawnOwner());
	if (MySweetBabyBoi)
	{
		MySweetBabyBoiMovement = MySweetBabyBoi->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MySweetBabyBoiMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MySweetBabyBoiMovement->Velocity);
		IsFalling = MySweetBabyBoiMovement->IsFalling();
		CharacterState = MySweetBabyBoi->GetCharacterState();
	}
}
