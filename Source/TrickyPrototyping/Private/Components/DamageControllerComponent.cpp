// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/DamageControllerComponent.h"
#include "Objects/EntityResource.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UDamageControllerComponent::UDamageControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UDamageControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthObject = NewObject<UEntityResource>(this, TEXT("HealthObject"));
	HealthObject->SetResourceData(HealthData);
	HealthObject->OnValueChanged.AddDynamic(this, &UDamageControllerComponent::BroadcastOnHealthChanged);
	HealthObject->OnValueMaxChanged.AddDynamic(this, &UDamageControllerComponent::BroadcastOnMaxHealthChanged);

	AActor* ComponentOwner = GetOwner();

	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UDamageControllerComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UDamageControllerComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UDamageControllerComponent::OnTakeRadialDamage);
	}
}

void UDamageControllerComponent::DecreaseHealth(const float Amount)
{
	if (Amount <= 0.f || GetIsDead()) return;

	HealthObject->DecreaseValue(Amount);
}

void UDamageControllerComponent::IncreaseHealth(const float Amount, const bool bClampToMax)
{
	if (Amount <= 0.f || GetIsDead()) return;

	HealthObject->IncreaseValue(Amount, bClampToMax);
}

void UDamageControllerComponent::DecreaseMaxHealth(const float Amount, const bool bClampCurrentHealth)
{
	if (Amount <= 0.f) return;

	HealthObject->DecreaseValueMax(Amount, bClampCurrentHealth);
}

void UDamageControllerComponent::IncreaseMaxHealth(const float Amount, const bool bClampCurrentHealth)
{
	if (Amount <= 0.f) return;

	HealthObject->IncreaseValueMax(Amount, bClampCurrentHealth);
}

void UDamageControllerComponent::SetHealthData(const FResourceData& NewData)
{
	HealthData = NewData;
	HealthObject->SetResourceData(HealthData);
}

void UDamageControllerComponent::BroadcastOnHealthChanged(const float NewHealth, const float DeltaHealth)
{
	OnHealthChanged.Broadcast(NewHealth, DeltaHealth);
}

void UDamageControllerComponent::BroadcastOnMaxHealthChanged(const float NewMaxHealth, const float DeltaMaxHealth)
{
	OnMaxHealthChanged.Broadcast(NewMaxHealth, DeltaMaxHealth);
}

void UDamageControllerComponent::SetGeneralDamageModifier(const float NewModifier)
{
	if (NewModifier <= 0.f) return;

	GeneralDamageModifier = NewModifier;
}

float UDamageControllerComponent::GetPointDamageModifier(AActor* Actor, const FName& BoneName)
{
	const ACharacter* Character = Cast<ACharacter>(Actor);

	if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.f;

	const UPhysicalMaterial* PhysMat = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();

	return PointDamageModifiers.Contains(PhysMat) ? PointDamageModifiers[PhysMat] : 1.f;
}

void UDamageControllerComponent::CalculateDamage_Implementation(const float Damage,
                                                                AActor* DamagedActor,
                                                                AController* Instigator,
                                                                AActor* Causer,
                                                                const UDamageType* DamageType)
{
	if (Damage <= 0.f) return;

	DecreaseHealth(Damage * GeneralDamageModifier);

	if (GetIsDead())
	{
		HealthObject->SetAutoIncreaseEnabled(false);
		OnDeath.Broadcast(Instigator, Causer, DamageType);
	}
}

void UDamageControllerComponent::OnTakeAnyDamage(AActor* DamageActor,
                                                 float Damage,
                                                 const UDamageType* DamageType,
                                                 AController* InstigatedBy,
                                                 AActor* DamageCauser)
{
}

void UDamageControllerComponent::OnTakePointDamage(AActor* DamagedActor,
                                                   float Damage,
                                                   AController* InstigatedBy,
                                                   FVector HitLocation,
                                                   UPrimitiveComponent* FHitComponent,
                                                   FName BoneName,
                                                   FVector ShotFromDirection,
                                                   const UDamageType* DamageType,
                                                   AActor* DamageCauser)
{
	float FinalDamage = Damage;

	if (bUsePointDamageModifier && PointDamageModifiers.Num() > 0)
	{
		FinalDamage *= GetPointDamageModifier(DamagedActor, BoneName);
	}

	CalculateDamage(FinalDamage, DamagedActor, InstigatedBy, DamageCauser, DamageType);
}

void UDamageControllerComponent::OnTakeRadialDamage(AActor* DamagedActor,
                                                    float Damage,
                                                    const UDamageType* DamageType,
                                                    FVector Origin,
                                                    FHitResult HitInfo,
                                                    AController* InstigatedBy,
                                                    AActor* DamageCauser)
{
	CalculateDamage(Damage, DamagedActor, InstigatedBy, DamageCauser, DamageType);
}
