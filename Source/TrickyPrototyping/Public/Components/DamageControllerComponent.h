// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/EntityResource.h"
#include "DamageControllerComponent.generated.h"

class UPhysicalMaterial;

/**
 * A base class for damage calculations.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, DeltaHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthChagedSignature, float, NewMaxHealth, float, DeltaMaxHeath);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeathSignature,
                                               AController*,
                                               DeathInstigator,
                                               AActor*,
                                               DeathCauser,
                                               const UDamageType*,
                                               DamageType);

UCLASS(ClassGroup=(TrickyPrototyping), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UDamageControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageControllerComponent();

protected:
	virtual void BeginPlay() override;

	// Health
public:
	UPROPERTY(BlueprintAssignable, Category="DamageController|Health")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintCallable, Category="DamageController|Health")
	FOnMaxHealthChagedSignature OnMaxHealthChanged;

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	float GetHealth() const { return HealthObject->GetValue(); }

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	float GetMaxHealth() const { return HealthObject->GetValueMax(); }

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	float GetNormalizedHealth() const { return HealthObject->GetNormalizedValue(); }

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	void DecreaseHealth(const float Amount);

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	void IncreaseHealth(const float Amount, const bool bClampToMax = true);

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	void DecreaseMaxHealth(const float Amount, const bool bClampCurrentHealth = true);

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	void IncreaseMaxHealth(const float Amount, const bool bClampCurrentHealth = true);

	UFUNCTION(BlueprintCallable, Category="DamageController|Health")
	bool GetIsDead() const { return GetHealth() <= 0.f; }

	UFUNCTION(BlueprintSetter, Category="DamageController|Health")
	void SetHealthData(UPARAM(DisplayName = "NewHeathData") const FResourceData& NewData);

	UFUNCTION(BlueprintGetter, Category="DamageController|Health")
	FResourceData GetHealthData() const { return HealthData; }

protected:
	void BroadcastOnHealthChanged(const float NewHealth, const float DeltaHealth);

	void BroadcastOnMaxHealthChanged(const float NewMaxHealth, const float DeltaMaxHealth);

	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetHealthData,
		BlueprintSetter=SetHealthData,
		Category="DamageController",
		meta=(AllowPrivateAccess="true"))
	FResourceData HealthData;

	UPROPERTY(BlueprintReadOnly, Category="DamageController")
	UEntityResource* HealthObject = nullptr;
private:
	// Damage calculation
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

	UFUNCTION(BlueprintGetter, Category="DamageController|Damage")
	float GetGeneralDamageModifier() const { return GeneralDamageModifier; }

	UFUNCTION(BlueprintSetter, Category="DamageController|Dmage")
	void SetGeneralDamageModifier(const float NewModifier);

	UFUNCTION(BlueprintPure, Category="DamageController|Damage")
	bool GetIsInvulnerable() const { return GeneralDamageModifier == 0.f; }

	UFUNCTION(BlueprintCallable, Category="DamageController|Damage")
	void SetIsInvulnerable(const bool bIsInvulnerable) { GeneralDamageModifier = bIsInvulnerable; }

	UFUNCTION(BlueprintGetter, Category="DamageController|Damage")
	float GetRadialDamageModifier() const { return RadialDamageModifier; }

	UFUNCTION(BlueprintSetter, Category="DamageController|Damage")
	void SetRadialDamageModifier(const float Value);

protected:
	/**
	 * Applies to all receiving damage.
	 */
	UPROPERTY(VisibleAnywhere,
		BlueprintGetter=GetGeneralDamageModifier,
		BlueprintSetter=SetGeneralDamageModifier,
		Category="DamageController|Damage")
	float GeneralDamageModifier = 1.f;

	/**
	 * Applies to radial damage only.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetRadialDamageModifier,
		BlueprintSetter=SetRadialDamageModifier,
		Category="DamageController|Damage",
		meta=(ClampMin="0"))
	float RadialDamageModifier = 1.0f;

	/**
	 * Determines if input damage depends on a part of body to which it was applied.
	 */
	UPROPERTY
	(EditDefaultsOnly, BlueprintReadWrite, Category="DamageController|Damage")
	bool bUsePointDamageModifier = false;

	/**
	 * Is used only for point damage.
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintReadWrite,
		Category="DamageController|Damage",
		meta=(EditCondition="bUsePointDamageModifier"))
	TMap<UPhysicalMaterial*, float> PointDamageModifiers;

	float GetPointDamageModifier(AActor* Actor, const FName& BoneName);

	UFUNCTION(BlueprintNativeEvent, Category="DamageController")
	void CalculateDamage(const float Damage,
	                     AActor* DamagedActor,
	                     AController* Instigator,
	                     AActor* Causer,
	                     const UDamageType* DamageType);

	virtual void CalculateDamage_Implementation(const float Damage,
	                                            AActor* DamagedActor,
	                                            AController* Instigator,
	                                            AActor* Causer,
	                                            const UDamageType* DamageType);

	UFUNCTION()
	virtual void OnTakeAnyDamage(AActor* DamageActor,
	                             float Damage,
	                             const UDamageType* DamageType,
	                             AController* InstigatedBy,
	                             AActor* DamageCauser);


	UFUNCTION()
	virtual void OnTakePointDamage(AActor* DamagedActor,
	                               float Damage,
	                               class AController* InstigatedBy,
	                               FVector HitLocation,
	                               class UPrimitiveComponent* FHitComponent,
	                               FName BoneName,
	                               FVector ShotFromDirection,
	                               const class UDamageType* DamageType,
	                               AActor* DamageCauser);

	UFUNCTION()
	virtual void OnTakeRadialDamage(AActor* DamagedActor,
	                                float Damage,
	                                const class UDamageType* DamageType,
	                                FVector Origin,
	                                FHitResult HitInfo,
	                                class AController* InstigatedBy,
	                                AActor* DamageCauser);

private:
};
