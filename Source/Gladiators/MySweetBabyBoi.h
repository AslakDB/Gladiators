// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterStates.h"
#include "MySweetBabyBoi.generated.h"

struct FInputActionValue;
struct FTimerHandle;
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
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* ActorHit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class UCameraComponent* Camera{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabyVariables")
		class USphereComponent* ColliderPickupWork;

	

	

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = input)
		class UInputAction* HealInput;

	/*This opens and closes inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* OpenInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* CloseInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		class UInputAction* PauseGame;


	//Input
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
	void HealPlayer(const FInputActionValue& input);

	void DodgeReset();
	void Movement();


	
	//bool GetIsAttack();
	int MaxHealth;
	int Health;
	bool GameIsPaused;
	bool InventoryIsOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BabyVariables | Animation")
		float InputX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BabyVariables | Animation")
		float InputY;


	// pickup functions
	void GetSword();
	void GetSpear();
	void GetAxe();
	void PickupSword(ASword* SwordEquipped);
	void PickupSpear(ASpear* SpearEquipped);
	void PickupAxe(AAxe* AxeEquipped);
	void PickupPotion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<ASword*> NearbySword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<ASpear*> NearbySpear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<AAxe*> NearbyAxe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<AHealthPotion*> Potions;
	void SpawnDefaultSword();
	void SpawnDefaultSpear();
	void SpawnDefaultAxe();

	//Overlap
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);



	// Blueprint Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CloseToSword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CloseToSpear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CloseToAxe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CloseToPotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HaveSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HaveAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool HaveSpear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAlive;


	//Dodge
	float DodgeDistance;
	float DodgeCooldown;
	float DodgeCooldownTime;
	FVector DodgeDirection;
	FTimerHandle Timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDodging;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void TakenDamage();

	UFUNCTION(BlueprintCallable)
		void SweetDeath();


	//Widgets
	UPROPERTY(VisibleAnywhere)
		class UInventoryWidget* InventoryWidget = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UInventoryWidget> TInventoryWidget;

	UPROPERTY(VisibleAnywhere)
		class UPauseMenuWidget* PauseWidget = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseMenuWidget> TPauseWidget;

	UPROPERTY(VisibleAnywhere)
		class UHealthBarComponent* HealthBarWidget;

	UPROPERTY()
		TSubclassOf<UHealthBarComponent> THealthBarWidget;


	//Weapon Ref
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		ASword* SpawnSword = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASword> Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		ASpear* SpawnSpear = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASpear> Spear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		AAxe* SpawnAxe = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAxe> Axe;

	

	UPROPERTY(VisibleAnywhere)
		class AEnemy* Enemy;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* SoundCue;

	

	float Yaw;
	float Pitch;

protected:


	///** Combat */
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	virtual void HandleDamage(float DamageAmount) override; 

private:
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> SwordClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> SpearClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> AxeClass;

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
