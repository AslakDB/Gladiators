// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicTest.generated.h"

UCLASS()
class GLADIATORS_API ABasicTest : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicTest();


	UPROPERTY(BlueprintReadWrite)
		float Health;

	UPROPERTY(BlueprintReadWrite)
		float MaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
		class UPlayerUserWidget* Widget = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerUserWidget> TWidget;

	UPROPERTY(VisibleAnywhere)
	class UBossWidget* BossWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBossWidget> TBossWidget;

	UFUNCTION(BlueprintCallable)
		void TestFunction();

};
