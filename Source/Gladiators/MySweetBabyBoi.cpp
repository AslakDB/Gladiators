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
	
}

// Called every frame
void AMySweetBabyBoi::Tick(float DeltaTime)
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
		EnhanceInputCom->BindAction(UseInput, ETriggerEvent::Started, this, &AMySweetBabyBoi::Use);
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

void AMySweetBabyBoi::PickupStuff()
{
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
	IsAttack = true;
}

void AMySweetBabyBoi::Dodge(const FInputActionValue& input)
{
}

void AMySweetBabyBoi::Use(const FInputActionValue& input)
{
}


