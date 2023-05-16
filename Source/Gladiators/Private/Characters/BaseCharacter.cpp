// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Weapons/Sword.h"
#include "Items/Weapons/Spear.h"
#include "Items/Weapons/Axe.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gladiators/MySweetBabyBoi.h"
#include "Gamemode/MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	IsAliveBool = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* ActorHit)
{
	if (IsAlive() && ActorHit)
	{
		DirectionalHitReact(ActorHit->GetActorLocation());
	}
	else Die();


	//PlayHitSound(ImpactPoint);
	//SpawnHitParticles(ImpactPoint);
}

void ABaseCharacter::Killed_Implementation()
{

}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die_Implementation()
{
	Cast<AMySweetBabyBoi>(IsAliveBool)->SweetAlive();
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::CheckNumberOfEnemies()
{
	//FString CurrentLevel = GetWorld()->GetMapName();

	///*if (IsRemainingEnemies())
	//{
	//	RemoveEnemies();
	//}*//*!IsRemainingEnemies() && */

	//if (CurrentLevel == "Gladiators1")
	//{
	//	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	//}
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}

	return Selection;
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::RemoveEnemies()
{
	EnemiesAlive--;
	GEngine->AddOnScreenDebugMessage(8, 8, FColor::Magenta, TEXT("Removed Enemies"));
}

void ABaseCharacter::ChangeLevel()
{
	UWorld* World = GetWorld();
	FString CurrentLevel = World->GetMapName();

	if (World && !IsEnemiesLeft())
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Gladiators1");
	}
	/*else if (World && !IsEnemiesLeft() && CurrentLevel == "Gladiators1")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "EnemyTest");
	}*/
	/*else if (World && !IsEnemiesLeft() && CurrentLevel == "Level2")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Level3");
	}*/
}

bool ABaseCharacter::IsEnemiesLeft()
{
	return EnemiesAlive > 0.f;
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

