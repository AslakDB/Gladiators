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

#include "InventoryWidget.h"
#include "Public/Hud/PauseMenuWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "PlayerUserWidget.h"
#include "Blueprint/UserWidget.h"


#include "Items/Weapons/Spear.h"
#include "Items/Weapons/Sword.h"
#include "Items/Weapons/Axe.h"

// Sets default values
AMySweetBabyBoi::AMySweetBabyBoi()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	ColliderPickup = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	ColliderPickup->SetupAttachment(GetRootComponent());
	ColliderPickup->bHiddenInGame = false;
	ColliderPickup->InitSphereRadius(100.f);
	ColliderPickup->OnComponentBeginOverlap.AddDynamic(this, &AMySweetBabyBoi::OnOverlapBegin);
	ColliderPickup->OnComponentEndOverlap.AddDynamic(this, &AMySweetBabyBoi::OnOverlapEnd);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	
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
	}

	if (InventoryWidget)
	{
		InventoryWidget->InventoryCount = 0;
	}


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

			Movement();

			AddControllerYawInput(Yaw);
			AddControllerPitchInput(Pitch);

			PauseWidget->RemoveFromParent();
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			PlayerController->SetShowMouseCursor(false);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		}
		
		else
		{
			PauseWidget->AddToViewport();
			PauseWidget->PauseMenuManager();
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			PlayerController->SetShowMouseCursor(true);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, PauseWidget);
		
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
		//EnhanceInputCom->BindAction(AttackInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::Attack);
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

void AMySweetBabyBoi::PickupSword()
{
	ASword* ItemToDestroy = NearbySword[0];
	NearbySword.RemoveAt(0);
	ItemToDestroy->Pickup();

	
}

void AMySweetBabyBoi::PickupSpear()
{
	ASpear* SpearToDestroy = NearbySpear[0];
	NearbySpear.RemoveAt(0);
	SpearToDestroy->Pickup();
}

void AMySweetBabyBoi::PickupAxe()
{
	AAxe* AxeToDestroy = NearbyAxe[0];
	NearbyAxe.RemoveAt(0);
	AxeToDestroy->Pickup();
}


bool AMySweetBabyBoi::GetIsAttack()
{
	return IsAttack;
}

void AMySweetBabyBoi::ResetAttack()
{
	IsAttack = false;
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
}

void AMySweetBabyBoi::Dodge(const FInputActionValue& input)
{
}

void AMySweetBabyBoi::Use(const FInputActionValue& input)
{
	if (NearbySword.Num() > 0)
	{
		PickupSword();
		return;
	}
	if (NearbySpear.Num() > 0)
	{
		PickupSpear();
		return;
	}
	if (NearbyAxe.Num() > 0)
	{
		PickupAxe();
		return;
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
			}

		}
	}




