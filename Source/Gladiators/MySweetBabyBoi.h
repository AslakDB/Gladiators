// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterStates.h"
#include "MySweetBabyBoi.generated.h"

struct FInputActionValue;
class UItems;
class ASword;
class ASpear;
class AAxe;
class AHealthPotion;

class AItem;
class UAnimMontage;


UCLASS()
class GLADIATORS_API AMySweetBabyBoi : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMySweetBabyBoi();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* ActorHit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class UCameraComponent* Camera{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class USphereComponent* ColliderPickupWork;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class UStaticMeshComponent* Mesh1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class UStaticMeshComponent* MeshSword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class UStaticMeshComponent* MeshSpear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class UStaticMeshComponent* MeshAxe;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* HeavyAttackInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = input)
		class UInputAction* DodgeInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = input)
		class UInputAction* UseInput;

	/*This opens and closes inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* OpenInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* CloseInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* PauseGame;

	/*TEST*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* FKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* AttackAction;


	
		bool GetIsAttack();

		int MaxHealth;
			int Health;
			bool GameIsPaused;


		bool InventoryIsOpen;



	UFUNCTION()
		void ResetAttack();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BabyVariables | Animation")
		float InputX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BabyVariables | Animation")
		float InputY;

	UFUNCTION()
		void GetSword();

	UFUNCTION()
		void GetSpear();

	UFUNCTION()
		void GetAxe();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<ASword*> NearbySword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<ASpear*> NearbySpear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<AAxe*> NearbyAxe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<AHealthPotion*> Potions;

private:
	void Forward(const FInputActionValue& input);
	void Right(const FInputActionValue& input);
	void MouseX(const FInputActionValue& input);
	void MouseY(const FInputActionValue& input);
	void Attack(const FInputActionValue& input);
	void HeavyAttack(const FInputActionValue& input);
	void Dodge(const FInputActionValue& input);
	void Use(const FInputActionValue& input);
	/*Functions for open and close inventory*/
	void OpenInv(const FInputActionValue& input);
	void CloseInv(const FInputActionValue& input);
	void PausedGame(const FInputActionValue& input);


	void Movement();
	void PickupSword();
	void PickupSpear();
	void PickupAxe();
	void PickupPotion();

	


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

	UPROPERTY(VisibleAnywhere)
		class UBossWidget* CyclopsWidget = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UBossWidget> TCyclopsWidget;

	UPROPERTY(VisibleAnywhere)
		class UBossWidget* ManticoreWidget = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UBossWidget> TManticoreWidget;

	UPROPERTY(VisibleAnywhere)
		class AEnemy* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		AActor* SpawnSword = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASword> Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		AActor* SpawnSpear = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASpear>Spear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		AActor* SpawnAxe = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAxe>Axe;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		AActor* Potions = nullptr;*/

	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attack")
		bool SwordAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool AxeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool SpearAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HeavySwordAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HeavyAxeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HeavySpearAttack;

	bool IsAttack;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HaveSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HaveAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HaveSpear;


	float Yaw;
	float Pitch;

protected:

	void FKeyPressed();
	//virtual void Attack() override;

	/** Combat */
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

};
