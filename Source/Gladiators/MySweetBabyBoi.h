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

	/*This opens and closes inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* OpenInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* CloseInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* PauseGame;


	UFUNCTION()
		bool GetIsAttack();

	UFUNCTION()
		void ResetAttack();

	
		bool InventoryIsOpen;


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
	/*Functions for open and close inventory*/
	void OpenInv(const FInputActionValue& input);
	void CloseInv(const FInputActionValue& input);
	void PausedGame(const FInputActionValue& input);

	bool IsAttack;
	float Yaw;
	float Pitch;


public:
	UPROPERTY(VisibleAnywhere)
		class UPlayerUserWidget* Widget = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UPlayerUserWidget> TWidget;

	UPROPERTY(VisibleAnywhere)
		class UInventoryWidget* InventoryWidget = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UInventoryWidget> TInventoryWidget;

	UPROPERTY(VisibleAnywhere)
		class UPauseMenuWidget* PauseWidget = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseMenuWidget> TPauseWidget;
};


