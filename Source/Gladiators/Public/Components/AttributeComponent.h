// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLADIATORS_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

private:
	// Current Health
	UPROPERTY(EditAnywhere, Category = "Actor Atributes")
		float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Atributes")
		float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 HealthPotions;

public:
	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();
	void AddHealthPotions(int32 NumberOfHealthPotions);
	void Heal();

	FORCEINLINE int32 GetHealthPotions() const { return HealthPotions; }
};
