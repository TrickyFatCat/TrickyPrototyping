// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PickupBase.generated.h"

class UInteractionSphereComponent;

/**
 * Base pickup class. Use for creating various pickups
 */

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UInteractionSphereComponent* InteractionTrigger = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USceneComponent* MeshScene = nullptr;

	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	bool ActivatePickup(APawn* TargetPawn);

	virtual bool ActivatePickup_Implementation(APawn* TargetPawn);

private:
	virtual bool ProcessInteraction_Implementation(APlayerController* PlayerController) override;

	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	bool bRequireInteraction = false;

	// Animation
private:
	FVector InitialLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	bool bAnimateRotation = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float RotationSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	bool bAnimatePosition = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float Amplitude = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float Frequency = 1.f;

	void AnimateRotation() const;
	void AnimatePosition() const;
};
