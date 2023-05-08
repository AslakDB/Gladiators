// Fill out your copyright notice in the Description page of Project Settings.


#include "MySweetBabyBoi.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameManager.h"
#include "Components/SphereComponent.h"
#include "BossWidget.h"
#include "Public/Enemy/Enemy.h"

#include "InventoryWidget.h"
#include "Public/Hud/PauseMenuWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "PlayerUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Enemy/Enemy.h"
#include "Items/HealthPotion.h"


#include "Items/Weapons/Spear.h"
#include "Items/Weapons/Sword.h"
#include "Items/Weapons/Axe.h"
#include "Kismet/GameplayStatics.h"

#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

// Sets default values
AMySweetBabyBoi::AMySweetBabyBoi()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	

	ColliderPickupWork = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	ColliderPickupWork->SetupAttachment(GetRootComponent());
	ColliderPickupWork->bHiddenInGame = false;
	ColliderPickupWork->InitSphereRadius(100.f);
	ColliderPickupWork->OnComponentBeginOverlap.AddDynamic(this, &AMySweetBabyBoi::OnOverlapBegin);
	ColliderPickupWork->OnComponentEndOverlap.AddDynamic(this, &AMySweetBabyBoi::OnOverlapEnd);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	MaxHealth = 100;
	Health = MaxHealth;

	Enemy = CreateDefaultSubobject<AEnemy>(TEXT("Enemy"));
	

}

// Called when the game starts or when spawned
void AMySweetBabyBoi::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngagableTarget"));


	GetCharacterMovement()->MaxWalkSpeed = 330.f;

	// Add the mapping context
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(MappingContext, 0);

		}
	}

	if (UWorld* World = GetWorld())
	{
		Widget = CreateWidget<UPlayerUserWidget>(World, TWidget);
		if (Widget)
		{
			Widget->AddToViewport(2);
		}

		PauseWidget = CreateWidget<UPauseMenuWidget>(World, TPauseWidget);
		if (PauseWidget)
		{
			PauseWidget->AddToViewport(3);
		}

		InventoryWidget = CreateWidget<UInventoryWidget>(World, TInventoryWidget);
		CyclopsWidget = CreateWidget<UBossWidget>(World, TCyclopsWidget);
		ManticoreWidget = CreateWidget<UBossWidget>(World, TManticoreWidget);
	}

	if (InventoryWidget)
	{
		InventoryWidget->InventoryCount = 0;
	}

	GameIsPaused = false;
}

// Called every frame
void AMySweetBabyBoi::Tick(float DeltaTime)
{
	if (PauseWidget)
	{
		if (!PauseWidget->Paused)
		{
			Super::Tick(DeltaTime);

			if (GetCharacterMovement()->IsFalling())
			{
				GetCharacterMovement()->bOrientRotationToMovement = false;
			}
			else
			{
				GetCharacterMovement()->bOrientRotationToMovement = true;
			}

			// Pulled this out to its own function
			Health -=1 * DeltaTime ;

			Movement();

			if (InventoryWidget)
			{
				InventoryWidget->ManageInventory();
			}

			AddControllerYawInput(Yaw);
			AddControllerPitchInput(Pitch);

			PauseWidget->RemoveFromParent();
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			PlayerController->SetShowMouseCursor(false);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			UGameplayStatics::SetGamePaused(this, false);

			if (Widget)
			{

				Widget->SetPlayerHealth(Health, MaxHealth);
			}
			if(CyclopsWidget && Enemy)
			{
			CyclopsWidget->SetBossHealth(Enemy->EnemyMaxHealth, Enemy->EnemyHealth);
			}
			if (ManticoreWidget && Enemy)
			{
			ManticoreWidget->SetBossHealth(Enemy->EnemyMaxHealth, Enemy->EnemyHealth);
			}
		}
		
		else
		{
			PauseWidget->AddToViewport();
			PauseWidget->PauseMenuManager();
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			PlayerController->SetShowMouseCursor(true);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, PauseWidget);
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}



// Called to bind functionality to input
void AMySweetBabyBoi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	APlayerController* PC = Cast<APlayerController>(GetController());
	if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhanceInputCom->BindAction(ForwardInput, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::Forward);
		EnhanceInputCom->BindAction(RightInput, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::Right);
		EnhanceInputCom->BindAction(ForwardInput, ETriggerEvent::Completed, this, &AMySweetBabyBoi::Forward);
		EnhanceInputCom->BindAction(RightInput, ETriggerEvent::Completed, this, &AMySweetBabyBoi::Right);
		EnhanceInputCom->BindAction(AttackInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::Attack);
		EnhanceInputCom->BindAction(HeavyAttackInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::HeavyAttack);
		EnhanceInputCom->BindAction(DodgeInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::Dodge);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Completed, this, &AMySweetBabyBoi::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Completed, this, &AMySweetBabyBoi::MouseY);
		EnhanceInputCom->BindAction(UseInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::Use);

		/*Code for input on open and close inventory*/
		EnhanceInputCom->BindAction(OpenInventory, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::OpenInv);
		EnhanceInputCom->BindAction(OpenInventory, ETriggerEvent::Completed, this, &AMySweetBabyBoi::OpenInv);
		EnhanceInputCom->BindAction(CloseInventory, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::CloseInv);
		EnhanceInputCom->BindAction(CloseInventory, ETriggerEvent::Completed, this, &AMySweetBabyBoi::CloseInv);
		EnhanceInputCom->BindAction(PauseGame, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::PausedGame);

		/*TEST*/
		EnhanceInputCom->BindAction(FKeyAction, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::FKeyPressed);
		//EnhanceInputCom->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::Attack);

	}
}

float AMySweetBabyBoi::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;
}

void AMySweetBabyBoi::GetHit_Implementation(const FVector& ImpactPoint, AActor* ActorHit)
{
	Super::GetHit_Implementation(ImpactPoint, ActorHit);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionState = EActionState::EAS_HitReaction;

	//PlayHitSound(ImpactPoint);
	//SpawnHitParticles(ImpactPoint);
}

void AMySweetBabyBoi::Movement()
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	//Movement
	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	//Getting the direction we're looking, and the right vector = cross product of forward and up vectors
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

	ForwardVector *= InputX;
	RightVector *= InputY;

	if (!FMath::IsNearlyZero(InputX))
	{
		AddMovementInput(ForwardVector);
	}
	if (!FMath::IsNearlyZero(InputY))
	{
		AddMovementInput(RightVector);
	}
}

void AMySweetBabyBoi::PickupSword(ASword* SwordEquipped)
{
	ASword* ItemToDestroy = NearbySword[0];
	NearbySword.RemoveAt(0);
	ItemToDestroy->Pickup();
	HaveSword = true;
	HaveAxe = false;
	HaveSpear = false;
	GetSword();

	//SwordEquipped->SetOwner(this);
	//SwordEquipped->SetInstigator(this);
	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedSword = SwordEquipped;
}

void AMySweetBabyBoi::PickupSpear(ASpear* SpearEquipped)
{
	ASpear* SpearToDestroy = NearbySpear[0];
	NearbySpear.RemoveAt(0);
	SpearToDestroy->Pickup();
	HaveSpear = true;
	HaveSword = false;
	HaveAxe = false;
	GetSpear();

	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedSpear = SpearEquipped;
}

void AMySweetBabyBoi::PickupAxe(AAxe* AxeEquipped)
{
	AAxe* AxeToDestroy = NearbyAxe[0];
	NearbyAxe.RemoveAt(0);
	AxeToDestroy->Pickup();
	HaveAxe = true;
	HaveSword = false;
	HaveSpear = false;
	GetAxe();
	
	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedAxe = AxeEquipped;
}

void AMySweetBabyBoi::GetSword()
{

	SpawnSword = GetWorld()->SpawnActor<AActor>(Sword, FVector(0, 0, 0), FRotator(90, 0, 0));
	if(SpawnSword)
	SpawnSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Sword"));
	

	
}

void AMySweetBabyBoi::GetSpear()
{
	SpawnSpear = GetWorld()->SpawnActor<AActor>(Spear, FVector(0, 0, 0), FRotator(90, 0, 0));
	if (SpawnSpear)
		SpawnSpear->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spear"));
	
	
}

void AMySweetBabyBoi::GetAxe()
{
	SpawnAxe = GetWorld()->SpawnActor<AActor>(Axe, FVector(0, 0, 0), FRotator(90, 0, 0));
	if (SpawnAxe)
		SpawnAxe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Axe"));
}

void AMySweetBabyBoi::PickupPotion()
{
	if (InventoryWidget && InventoryWidget->InventoryCount != 3)
	{
	GEngine->AddOnScreenDebugMessage(8, 8, FColor::Magenta, TEXT("Picking up potions"));
	AHealthPotion* PotionToDestroy = Potions[0];
	Potions.RemoveAt(0);
	PotionToDestroy->Pickup();
		InventoryWidget->InventoryCount++;
	}
}

bool AMySweetBabyBoi::GetIsAttack()
{
	return IsAttack;
}

void AMySweetBabyBoi::ResetAttack()
{
	IsAttack = false;
	SwordAttack = false;
	AxeAttack = false;
	SpearAttack = false;
	HeavySwordAttack = false;
	HeavyAxeAttack = false;
	HeavySpearAttack = false;
	
}

void AMySweetBabyBoi::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ASword>())
	{
		NearbySword.Add(Cast<ASword>(OtherActor));
	}
	if (OtherActor->IsA<ASpear>())
	{
		NearbySpear.Add(Cast<ASpear>(OtherActor));
	}
	if (OtherActor->IsA<AAxe>())
	{
		NearbyAxe.Add(Cast<AAxe>(OtherActor));
	}
	if (OtherActor->IsA<AHealthPotion>())
	{
		Potions.Add(Cast<AHealthPotion>(OtherActor));
	}
}

void AMySweetBabyBoi::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{	
	if (OtherActor->IsA<ASword>())
	{
		NearbySword.Remove(Cast<ASword>(OtherActor));
		
	}
	if(OtherActor->IsA<ASpear>())
	{
		NearbySpear.Remove(Cast<ASpear>(OtherActor));
	}
	if(OtherActor->IsA<AAxe>())
	{
		NearbyAxe.Remove(Cast<AAxe>(OtherActor));
	}
	if (OtherActor->IsA<AHealthPotion>())
	{
		Potions.Remove(Cast<AHealthPotion>(OtherActor));
	}
}

void AMySweetBabyBoi::Forward(const FInputActionValue& input)
{
	InputX = input.Get<float>();
}

void AMySweetBabyBoi::Right(const FInputActionValue& input)
{
	InputY = input.Get<float>();
}

void AMySweetBabyBoi::MouseX(const FInputActionValue& input)
{
	Yaw = input.Get<float>();
}

void AMySweetBabyBoi::MouseY(const FInputActionValue& input)
{
	Pitch = input.Get<float>();
}

void AMySweetBabyBoi::Attack(const FInputActionValue& input)
{
	if (CanAttack())
	{
		IsAttack = true;
		if (HaveSword)
		{
			SwordAttack = true;
		}
		if (HaveAxe)
		{
			AxeAttack = true;
		}
		if (HaveSpear)
		{
			SpearAttack = true;
		}
	}
}

void AMySweetBabyBoi::HeavyAttack(const FInputActionValue& input)
{
	IsAttack = true;
	if (HaveSword)
	{
		HeavySwordAttack = true;
	}
	if (HaveAxe)
	{
		HeavyAxeAttack = true;
	}
	if (HaveSpear)
	{
		HeavySpearAttack = true;
	}
}


void AMySweetBabyBoi::Dodge(const FInputActionValue& input)
{
}

void AMySweetBabyBoi::Use(const FInputActionValue& input)
{
	ASword* OverlappingSword = Cast<ASword>(OverlappingItem);
	ASpear* OverlappingSpear = Cast<ASpear>(OverlappingItem);
	AAxe* OverlappingAxe = Cast<AAxe>(OverlappingItem);

	if (NearbySword.Num() > 0)
	{
		PickupSword(OverlappingSword);
		SpawnDefaultSword();
		return;
	}
	if (NearbySpear.Num() > 0)
	{
		PickupSpear(OverlappingSpear);
		SpawnDefaultSpear();
		return;
	}
	if (NearbyAxe.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(9, 8, FColor::Magenta, TEXT("Axe is nearby"));

		PickupAxe(OverlappingAxe);
		SpawnDefaultAxe();
		return;
	}
	if (Potions.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(8, 8, FColor::Magenta, TEXT("Potions nearby"));

		PickupPotion();
	}
	if (InventoryWidget->IsInViewport() && InventoryWidget->InventoryCount !=0)
	{
		Health = MaxHealth;
		InventoryWidget->InventoryCount--;
	}
}

void AMySweetBabyBoi::OpenInv(const FInputActionValue& input)
{
	if (InventoryWidget)
	{
		InventoryWidget->AddToViewport(1);
	}
	
}

void AMySweetBabyBoi::CloseInv(const FInputActionValue& input)
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
	}
}

void AMySweetBabyBoi::PausedGame(const FInputActionValue & input)
	{
		if (PauseWidget)
		{
			if (!PauseWidget->Paused)
			{
				PauseWidget->Paused = true;
				GEngine->AddOnScreenDebugMessage(3, 6, FColor::Blue, TEXT("P pressed"));
				GameIsPaused = true;
			}

		}
	}

void AMySweetBabyBoi::FKeyPressed()
{
	/*AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}*/

	ASword* OverlappingSword = Cast<ASword>(OverlappingItem);
	if (OverlappingSword)
	{
		EquipSword(OverlappingSword);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

//void AMySweetBabyBoi::Attack()
//{
//	Super::Attack();
//	/*if (CanAttack())
//	{
//		PlayAttackMontage();
//		ActionState = EActionState::EAS_Attacking;
//	}*/
//
//	IsAttack = true;
//	if (HaveSword)
//	{
//		SwordAttack = true;
//	}
//	if (HaveAxe)
//	{
//		AxeAttack = true;
//	}
//	if (HaveSpear)
//	{
//		SpearAttack = true;
//	}
//}

void AMySweetBabyBoi::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("Sword"), this, this);
	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AMySweetBabyBoi::EquipSword(ASword* SwordEquip)
{
	SwordEquip->Equip(GetMesh(), FName("Sword"), this, this);
	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedSword = SwordEquip;
}

void AMySweetBabyBoi::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AMySweetBabyBoi::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AMySweetBabyBoi::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AMySweetBabyBoi::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AMySweetBabyBoi::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
}

void AMySweetBabyBoi::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedWeapon;
}

void AMySweetBabyBoi::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AMySweetBabyBoi::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMySweetBabyBoi::SpawnDefaultSword()
{
	UWorld* World = GetWorld();
	if (World && SwordClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(SwordClass);
		DefaultWeapon->Equip(GetMesh(), FName("SwordSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AMySweetBabyBoi::SpawnDefaultSpear()
{
	UWorld* World = GetWorld();
	if (World && SpearClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(SpearClass);
		DefaultWeapon->Equip(GetMesh(), FName("SpearSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AMySweetBabyBoi::SpawnDefaultAxe()
{
	UWorld* World = GetWorld();
	if (World && AxeClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(AxeClass);
		DefaultWeapon->Equip(GetMesh(), FName("AxeSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}
