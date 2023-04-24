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
#include "InventoryWidget.h"
#include "Public/Hud/PauseMenuWidget.h"

#include "PlayerUserWidget.h"
#include "Blueprint/UserWidget.h"

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
	if (InventoryWidget)
	{
		InventoryWidget->InventoryCount = 0;
	}
}

// Called every frame
void AMySweetBabyBoi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
			if (InventoryWidget)
			{
				InventoryWidget->ManageInventory();
			}


			Movement();
			if (Controller)
			{


				AddControllerYawInput(Yaw);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, TEXT("Controller Is false"));
			}
			AddControllerPitchInput(Pitch);
			PauseWidget->RemoveFromParent();
		}
		else
		{

			PauseWidget->AddToViewport();
			PauseWidget->PauseMenuManager();
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
		EnhanceInputCom->BindAction(DodgeInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::Dodge);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Triggered, this, &AMySweetBabyBoi::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Completed, this, &AMySweetBabyBoi::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Completed, this, &AMySweetBabyBoi::MouseY);
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
	
	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	
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

bool AMySweetBabyBoi::GetIsAttack()
{
	return IsAttack;
}

void AMySweetBabyBoi::ResetAttack()
{
	IsAttack = false;
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
	
}

void AMySweetBabyBoi::Dodge(const FInputActionValue& input)
{
}

void AMySweetBabyBoi::OpenInv(const FInputActionValue& input)
{
	
	if (InventoryWidget)
	{
		InventoryWidget->AddToViewport(1);
	}
	
}
inline void AMySweetBabyBoi::CloseInv(const FInputActionValue& input)
{
	
	if (InventoryWidget )
	{
		InventoryWidget->RemoveFromParent();
	}
}

void AMySweetBabyBoi::PausedGame(const FInputActionValue& input)
{
	if (PauseWidget)
	{
		if (!PauseWidget->Paused)
		{
			PauseWidget->Paused = true;
		}
		
	}
}

