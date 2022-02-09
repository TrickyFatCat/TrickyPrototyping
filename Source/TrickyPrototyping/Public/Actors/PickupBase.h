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

//TODO Implement logic which allows to choose either to destroy the pickup on activation or deactivate it.

UCLASS()
class TRICKYPROTOTYPING_API APickupBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	APickupBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USceneComponent* PickupRoot = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UInteractionSphereComponent* InteractionTrigger = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USceneComponent* MeshScene = nullptr;
	
	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	bool ActivatePickup(AActor* TargetActor);

	virtual bool ActivatePickup_Implementation(AActor* TargetActor);

	virtual void DestroyPickup();

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
