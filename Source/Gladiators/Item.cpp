// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	//Mesh->SetupAttachment(GetRootComponent());

	ItemWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemWidget"));
	ItemWidget->SetupAttachment(GetRootComponent());

	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	if (ItemWidget)
	{
		GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Green, FString("invisable"));
		ItemWidget->SetVisibility(false);
		GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Red, FString("invisable"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 60.f, FColor::Cyan, FString("ItemWidget Null error"));
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();

	if(GetWorld())
	{
		if (Character)
		{
			//FVector CurrentLocation = GetActorLocation();
			FVector CharacterLocation = Character->GetActorLocation();

			float Distance = FVector::Distance(CurrentLocation, CharacterLocation);

			bool close = true;
			if(Distance < 200)
			{
				ItemWidget->SetVisibility(true);
				GEngine->AddOnScreenDebugMessage(4, 60.f, FColor::Cyan, FString("Bool is true"));
			}
			else
			{
				ItemWidget->SetVisibility(false);
				GEngine->AddOnScreenDebugMessage(4, 60.f, FColor::Cyan, FString("Bool is false"));
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(5, 60.f, FColor::Blue, FString("Character does not exist"));
		}
	}
}

