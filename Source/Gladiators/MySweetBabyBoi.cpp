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
#include "Engine/EngineTypes.h"

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

	MaxHealth = 100;
	Health = MaxHealth;
	DodgeCooldownTime = 2.f;
	DodgeDistance = 500;

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
			//PlayerController->SetShowMouseCursor(false);
			//UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
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

	}
}


void AMySweetBabyBoi::Movement()
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

void AMySweetBabyBoi::Dodge(const FInputActionValue& input)
{

	//if (DodgeCooldown <= 0.f)
	//{
	//	// Reset the dodge cooldown
	//	DodgeCooldown = DodgeCooldownTime;

	//	// Get the character's movement component
	//	UCharacterMovementComponent* movementComponent = GetCharacterMovement();

	//	if (movementComponent)
	//	{
	//		// Set the movement mode to flying to allow for more agile movement
	//		movementComponent->SetMovementMode(MOVE_Flying);

	//		// Apply movement input in the dodge direction
	//		FVector movementInput = DodgeDirection * DodgeDistance;
	//		movementComponent->AddInputVector(movementInput);
	//	}
	//	
	//}
	//
	//GEngine->AddOnScreenDebugMessage(8, 8, FColor::Magenta, TEXT("Dodge Called"));


}

void AMySweetBabyBoi::DodgeReset()
{

	/*UCharacterMovementComponent* movementComp = GetCharacterMovement();

	movementComp->StopMovementImmediately();
	GEngine->AddOnScreenDebugMessage(8, 8, FColor::Magenta, TEXT("Koffer stoppa du ikke"));*/
}

void AMySweetBabyBoi::PickupSword()
{
	ASword* SwordToDestroy = NearbySword[0];
	NearbySword.Empty();
	SwordToDestroy->Pickup();
	GetSword();
	
}

void AMySweetBabyBoi::PickupSpear()
{
	ASpear* SpearToDestroy = NearbySpear[0];
	NearbySpear.Empty();
	SpearToDestroy->Pickup();
	GetSpear();
}

void AMySweetBabyBoi::PickupAxe()
{
	AAxe* AxeToDestroy = NearbyAxe[0];
	NearbyAxe.Empty();
	AxeToDestroy->Pickup();
	GetAxe();
}

void AMySweetBabyBoi::GetSword()
{

	SwordRef = GetWorld()->SpawnActor<ASword>(Sword, FVector(0, 0, 0), FRotator(90, 0, 0));
	if(SwordRef)
	SwordRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Sword"));


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

	if (SwordRef)
	{
		FDetachmentTransformRules* Detachment;
		SwordRef->DetachFromActor(Detachment->KeepWorldTransform);
		SwordRef->Collider->SetSimulatePhysics(true);
		SwordRef = nullptr;
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

	
	if (SwordRef)
	{
		FDetachmentTransformRules* Detachment;
		SwordRef->DetachFromActor(Detachment->KeepWorldTransform);
		SwordRef->Collider->SetSimulatePhysics(true);
		SwordRef = nullptr;
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
	if (OtherActor->IsA<ASpear>())
	{
		NearbySpear.Add(Cast<ASpear>(OtherActor));
	}
	if (OtherActor->IsA<AAxe>())
	{
		NearbyAxe.Add(Cast<AAxe>(OtherActor));
	}
	if (OtherActor->IsA<ASword>())
	{
		NearbySword.Add(Cast<ASword>(OtherActor));
	}
	
	
	if (OtherActor->IsA<AHealthPotion>())
	{
		Potions.Add(Cast<AHealthPotion>(OtherActor));
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
	IsAttack = true;
	if(HaveSword)
	{
		SwordAttack = true;
	}
	if(HaveAxe)
	{
		AxeAttack = true;
	}
	if(HaveSpear)
	{
		SpearAttack = true;
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


void AMySweetBabyBoi::Use(const FInputActionValue& input)
{
	if (NearbySword.Num() > 0)
	{
		PickupSword();
		//return;
		NearbySword.Empty();
	}
	if (NearbySpear.Num() > 0)
	{
		PickupSpear();
		//return;
		NearbySpear.Empty();
	}
	if (NearbyAxe.Num() > 0)
	{

		PickupAxe();
		//return;
		NearbyAxe.Empty();
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




