// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PickupBase.generated.h"

class UInteractionSphereComponent;
class USoundCue;

/**
 * Base pickup class. Use for creating various pickups
 */

UCLASS(BlueprintType, Blueprintable)
class TRICKYPROTOTYPING_API APickupBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	APickupBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintGetter, Category="Pickup")
	bool GetDestroyOnEffectActivation() const { return bDestroyOnEffectActivation; }

	UFUNCTION(BlueprintSetter, Category="Pickup")
	void SetDestroyOnEffectActivation(const bool Value) { bDestroyOnEffectActivation = Value; }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Pickup")
	void ActivatePickup();

	virtual void ActivatePickup_Implementation();
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USceneComponent* PickupRoot = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UInteractionSphereComponent* InteractionTrigger = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USceneComponent* MeshScene = nullptr;
	
	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	bool ActivatePickupEffect(AActor* TargetActor);

	virtual bool ActivatePickupEffect_Implementation(AActor* TargetActor);

	virtual void DestroyPickup();

	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void DeactivatePickup();

	virtual void DeactivatePickup_Implementation();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pickup", meta=(AllowPrivateAccess="true"))
	USoundCue* PickupSound = nullptr;
	
	virtual bool ProcessInteraction_Implementation(AActor* TargetActor) override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	/**
	 * Determines if the pickup requires interaction.
	 * Use this parameter instead of the similar one in the interaction trigger.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	bool bRequireInteraction = false;

	/**
	 * Determine if the pickup requires the line of sight check for interaction.
	 * Use this parameter instead of the similar on in the interaction trigger.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Pickup", meta=(EditCondition="bRequireInteraction"))
	bool bRequireLineOfSight = false;
	
	/**
	 * If true, the pickup will be destroyed, else it'll be deactivated and hidden in game.
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetDestroyOnEffectActivation,
		BlueprintSetter=SetDestroyOnEffectActivation,
		Category="Pickup",
		meta=(AllowPrivateAccess="true"))
	bool bDestroyOnEffectActivation = true;

	// Animation
private:
	FVector InitialLocation{FVector::ZeroVector};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	bool bAnimateRotation = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float RotationSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	bool bAnimatePosition = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float Amplitude = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float Frequency = 1.f;

	void AnimateRotation() const;
	void AnimatePosition() const;
};
