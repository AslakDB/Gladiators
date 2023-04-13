// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MySweetBabyBoi.generated.h"

struct FInputActionValue;

UCLASS()
class GLADIATORS_API AMySweetBabyBoi : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMySweetBabyBoi();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class UCameraComponent* Camera{ nullptr };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* ForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* RightInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* MouseXInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* MouseYInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* AttackInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = input)
		class UInputAction* DodgeInput;

	UFUNCTION()
		bool GetIsAttack();

	UFUNCTION()
		void ResetAttack();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BabyVariables | Animation")
		float InputX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BabyVariables | Animation")
		float InputY;

private:
	void Forward(const FInputActionValue& input);
	void Right(const FInputActionValue& input);
	void MouseX(const FInputActionValue& input);
	void MouseY(const FInputActionValue& input);
	void Attack(const FInputActionValue& input);
	void Dodge(const FInputActionValue& input);
	void Movement();


	bool IsAttack;
	float Yaw;
	float Pitch;

};
