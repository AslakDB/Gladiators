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
#include "Public/Enemy/Enemy.h"
#include "Engine/EngineTypes.h"
#include "Sound/SoundCue.h"
#include "InventoryWidget.h"
#include "Public/Hud/PauseMenuWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Public/Hud/Health/HealthBarComponent.h"


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
#include "Components/AttributeComponent.h"
#include "Hud/Health/HealthBar.h"
#include "Public/Hud/Health/HealthBar.h"

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
	
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));

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
	DodgeCooldownTime = 1.f;
	DodgeDistance = 550;

	
	IsAlive = true;
	Enemy = CreateDefaultSubobject<AEnemy>(TEXT("Enemy"));
}

// Called when the game starts or when spawned
void AMySweetBabyBoi::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngagableTarget"));


	GetCharacterMovement()->MaxWalkSpeed = 425.f;
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
		
		PauseWidget = CreateWidget<UPauseMenuWidget>(World, TPauseWidget);
		InventoryWidget = CreateWidget<UInventoryWidget>(World, TInventoryWidget);
	}

	if (InventoryWidget)
	{
		InventoryWidget->InventoryCount = 0;
	}

	if (InventoryWidget){InventoryWidget->InventoryCount = 0;}
	GameIsPaused = false;
}

// Called every frame
void AMySweetBabyBoi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DodgeCooldown -= DeltaTime;

	if (PauseWidget)
	{
		if (!PauseWidget->Paused)
		{
			if (GetCharacterMovement()->IsFalling())
			{
				GetCharacterMovement()->bOrientRotationToMovement = false;
			}
			else
			{
				GetCharacterMovement()->bOrientRotationToMovement = true;
			}

			HealthBarWidget->SetHealthBarPercent(Attributes->GetHealthPercent());

			if (!HealthBarWidget->HealthBarWidget)
			{
				HealthBarWidget->HealthBarWidget = Cast<UHealthBar>(HealthBarWidget->GetUserWidgetObject());
			}
			else if (HealthBarWidget->HealthBarWidget)
			{
				HealthBarWidget->HealthBarWidget->AddToViewport(99);
			}
			

			Movement();

			if (InventoryWidget)
			{
				InventoryWidget->ManageInventory();
			}

			AddControllerYawInput(Yaw);
			AddControllerPitchInput(Pitch);
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			PlayerController->SetShowMouseCursor(false);
			PauseWidget->RemoveFromParent();
			
			
			UGameplayStatics::SetGamePaused(this, false);
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
		if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
		{
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
			EnhanceInputCom->BindAction(HealInput, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::HealPlayer);
		}
	
}

float AMySweetBabyBoi::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	TakenDamage();
	HandleDamage(DamageAmount);
	return DamageAmount;
	
}

void AMySweetBabyBoi::GetHit_Implementation(const FVector& ImpactPoint, AActor* ActorHit)
{
	Super::GetHit_Implementation(ImpactPoint, ActorHit);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionState = EActionState::EAS_HitReaction;
}

void AMySweetBabyBoi::Die_Implementation()
{
	Super::Die_Implementation();
	IsAlive = false;
}

void AMySweetBabyBoi::Movement()
{
	if(IsAlive)
	{
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
	
}

void AMySweetBabyBoi::Dodge(const FInputActionValue& input)
{
	if(IsAlive)
	{
		if (DodgeCooldown <= 0.f)
		{
			DodgeCooldown = DodgeCooldownTime;
			UCharacterMovementComponent* movementComponent = GetCharacterMovement();

			if (movementComponent)
			{
				if (movementComponent->MovementMode != MOVE_Flying)
				{
					movementComponent->SetMovementMode(MOVE_Flying);
					FVector movementInput = DodgeDirection * DodgeDistance;
					movementComponent->AddInputVector(movementInput);
					IsDodging = true;
					GetWorld()->GetTimerManager().SetTimer(Timer, this, &AMySweetBabyBoi::DodgeReset, 0.5f, false);
				}
			}
		}
	}
	
}

void AMySweetBabyBoi::DodgeReset()
{
	UCharacterMovementComponent* movementComponent = GetCharacterMovement();
	movementComponent->SetMovementMode(MOVE_Walking);
	IsDodging = false;
}

void AMySweetBabyBoi::PickupSword(ASword* SwordEquipped)
{
	ASword* SwordToDestroy = NearbySword[0];
	NearbySword.Empty();
	SwordToDestroy->Pickup();
	GetSword();
	CloseToSword = false;
	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedSword = SwordEquipped;
}

void AMySweetBabyBoi::PickupSpear(ASpear* SpearEquipped)
{
	ASpear* SpearToDestroy = NearbySpear[0];
	NearbySpear.Empty();
	SpearToDestroy->Pickup();
	GetSpear();
	CloseToSpear = false;
	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedSpear = SpearEquipped;
}

void AMySweetBabyBoi::PickupAxe(AAxe* AxeEquipped)
{
	AAxe* AxeToDestroy = NearbyAxe[0];
	NearbyAxe.Empty();
	AxeToDestroy->Pickup();
	GetAxe();
	CloseToAxe = false;
	CharacterState = ECharacterState::ECS_EquippedWeapon;
	OverlappingItem = nullptr;
	EquippedAxe = AxeEquipped;
}

void AMySweetBabyBoi::GetSword()
{

	SpawnSword = GetWorld()->SpawnActor<ASword>(Sword, FVector(0, 0, 0), FRotator(90, 0, 0));
	if(SpawnSword)
		SpawnSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Sword"));


	if(SpawnSpear)
	{
		FDetachmentTransformRules* Detachment;
		SpawnSpear->DetachFromActor(Detachment->KeepWorldTransform);
		SpawnSpear->SpearCollider->SetSimulatePhysics(true);
		SpawnSpear = nullptr;

	}

	if (SpawnAxe)
	{
		FDetachmentTransformRules* Detachment;
		SpawnAxe->DetachFromActor(Detachment->KeepWorldTransform);
		SpawnAxe->AxeCollider->SetSimulatePhysics(true);
		SpawnAxe = nullptr;
	}

	HaveAxe = false;
	HaveSword = true;
	HaveSpear = false;
	
}

void AMySweetBabyBoi::GetSpear()
{
	SpawnSpear = GetWorld()->SpawnActor<ASpear>(Spear, FVector(0, 0, 0), FRotator(90, 0, 0));
	if (SpawnSpear)
		SpawnSpear->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spear"));

	if (SpawnSword)
	{
		FDetachmentTransformRules* Detachment;
		SpawnSword->DetachFromActor(Detachment->KeepWorldTransform);
		SpawnSword->Collider->SetSimulatePhysics(true);
		SpawnSword = nullptr;
	}

	if (SpawnAxe)
	{
		FDetachmentTransformRules* Detachment;
		SpawnAxe->DetachFromActor(Detachment->KeepWorldTransform);
		SpawnAxe->AxeCollider->SetSimulatePhysics(true);
		SpawnAxe = nullptr;
	}



	HaveAxe = false;
	HaveSword = false;
	HaveSpear = true;
	
}

void AMySweetBabyBoi::GetAxe()
{
	SpawnAxe = GetWorld()->SpawnActor<AAxe>(Axe, FVector(0, 0, 0), FRotator(90, 0, 0));
	if (SpawnAxe)
		SpawnAxe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Axe"));
	
	
	if (SpawnSword)
	{
		FDetachmentTransformRules* Detachment;
		SpawnSword->DetachFromActor(Detachment->KeepWorldTransform);
		SpawnSword->Collider->SetSimulatePhysics(true);
		SpawnSword = nullptr;
	}

	if (SpawnSpear)
	{
		FDetachmentTransformRules* Detachment;
		SpawnSpear->DetachFromActor(Detachment->KeepWorldTransform);
		SpawnSpear->SpearCollider->SetSimulatePhysics(true);
		SpawnSpear = nullptr;

	}

	HaveAxe = true;
	HaveSword = false;
	HaveSpear = false;
}


void AMySweetBabyBoi::PickupPotion()
{
	if (InventoryWidget && InventoryWidget->InventoryCount != 3)
	{
		AHealthPotion* PotionToDestroy = Potions[0];
		Potions.RemoveAt(0);
		PotionToDestroy->Pickup();
		InventoryWidget->InventoryCount++;
		CloseToPotion = false;
	}
}

void AMySweetBabyBoi::SweetDeath()
{
	UGameplayStatics::OpenLevel(this, "DeathScreenLevel");
}

void AMySweetBabyBoi::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ASpear>())
	{
		NearbySpear.Add(Cast<ASpear>(OtherActor));
		CloseToSword = false;
		CloseToSpear = true;
		CloseToAxe = false;
	}
	if (OtherActor->IsA<AAxe>())
	{
		NearbyAxe.Add(Cast<AAxe>(OtherActor));
		CloseToSword = false;
		CloseToAxe = true;
		CloseToSpear = false;
	}
	if (OtherActor->IsA<ASword>())
	{
		NearbySword.Add(Cast<ASword>(OtherActor));
		CloseToSword = true;
		CloseToSpear = false;
		CloseToAxe = false;
	}
	
	
	if (OtherActor->IsA<AHealthPotion>())
	{
		Potions.Add(Cast<AHealthPotion>(OtherActor));
		CloseToPotion = true;
	}
}

void AMySweetBabyBoi::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AAxe>())
	{
		NearbyAxe.Remove(Cast<AAxe>(OtherActor));
	}
	if (OtherActor->IsA<ASword>())
	{
		NearbySword.Remove(Cast<ASword>(OtherActor));
		
	}
	if(OtherActor->IsA<ASpear>())
	{
		NearbySpear.Remove(Cast<ASpear>(OtherActor));
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
	if(IsAlive)
	{
		if (CanAttack())
		{
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
	
}

void AMySweetBabyBoi::HeavyAttack(const FInputActionValue& input)
{
	if(IsAlive)
	{
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
	
}


void AMySweetBabyBoi::Use(const FInputActionValue& input)
{
	ASword* OverlappingSword = Cast<ASword>(OverlappingItem);
	ASpear* OverlappingSpear = Cast<ASpear>(OverlappingItem);
	AAxe* OverlappingAxe = Cast<AAxe>(OverlappingItem);

	if (NearbySword.Num() > 0)
	{
		
		PickupSword(OverlappingSword);
		NearbySword.Empty();
		SpawnDefaultSword();
		return;
	}
	
	if (NearbySpear.Num() > 0)
	{
		
		PickupSpear(OverlappingSpear);
		NearbySpear.Empty();
		SpawnDefaultSpear();
		return;
	}
	if (NearbyAxe.Num() > 0)
	{
		
		PickupAxe(OverlappingAxe);
		NearbyAxe.Empty();
		SpawnDefaultAxe();
		return;
	}
	if (Potions.Num() > 0)
	{
		PickupPotion();
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

void AMySweetBabyBoi::HealPlayer(const FInputActionValue& input)
{
	if (InventoryWidget->InventoryCount != 0)
	{
		UGameplayStatics::PlaySound2D(this, SoundCue, 0.5, 1);
		Attributes->Heal();
		InventoryWidget->InventoryCount--;
	}
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

void AMySweetBabyBoi::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void AMySweetBabyBoi::SpawnDefaultSword()
{
	UWorld* World = GetWorld();
	if (World && SwordClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(SwordClass);
		DefaultWeapon->Equip(GetMesh(), FName("Sword"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AMySweetBabyBoi::SpawnDefaultSpear()
{
	UWorld* World = GetWorld();
	if (World && SpearClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(SpearClass);
		DefaultWeapon->Equip(GetMesh(), FName("Spear"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AMySweetBabyBoi::SpawnDefaultAxe()
{
	UWorld* World = GetWorld();
	if (World && AxeClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(AxeClass);
		DefaultWeapon->Equip(GetMesh(), FName("Axe"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}
