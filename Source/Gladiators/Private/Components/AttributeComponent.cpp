// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

#include "Gladiators/MySweetBabyBoi.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool UAttributeComponent::IsAlive()
{

	return Health > 0.f;
}

void UAttributeComponent::AddHealthPotions(int32 NumberOfHealthPotions)
{
	HealthPotions += NumberOfHealthPotions;
	Health = FMath::Clamp(Health + 20, 0.f, MaxHealth);
}

void UAttributeComponent::Heal()
{
	Health += MaxHealth * 0.5;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

