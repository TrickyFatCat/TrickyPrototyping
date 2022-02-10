// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/AnimatedActor.h"
#include "Interfaces/InteractionInterface.h"
#include "DoorBase.generated.h"

class UInteractionBoxComponent;
class UKeyType;

/**
 * A base door class
 */

UENUM(Blueprinttype)
enum class EDoorType : uint8
{
	Manual,
	Interactive,
	Auto
};

UCLASS()
class TRICKYPROTOTYPING_API ADoorBase : public AAnimatedActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADoorBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void Disable_Implementation() override;

	virtual void Enable_Implementation() override;

protected:
	virtual void FinishAnimation_Implementation() override;

	// DoorType
public:
	UFUNCTION(BlueprintGetter)
	EDoorType GetDoorType() const { return DoorType; }

	UFUNCTION(BlueprintSetter)
	void SetDoorType(const EDoorType Value);

private:
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetDoorType,
		BlueprintSetter=SetDoorType,
		Category="Door",
		meta=(AllowPrivateAccess="true"))
	EDoorType DoorType = EDoorType::Manual;

	/**
	 * Determine if the door requires the line of sight check for interaction.
	 * Works only with Interactive doors.
	 * Use this parameter instead of the similar one in the interaction trigger.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="Door",
		meta=(AllowPrivateAccess="true", EditCondition="DoorType == EDoorType::Interactive"))
	bool bRequireLineOfSight = false;

	void SwitchDoorType() const;

	// Trigger
protected:
	UFUNCTION()
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp,
	                                   int32 OtherBodyIndex,
	                                   bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                                 AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp,
	                                 int32 OtherBodyIndex);

	UInteractionBoxComponent* GetDoorTrigger() const { return DoorTrigger; }

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* DoorRoot = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UInteractionBoxComponent* DoorTrigger = nullptr;

	// Interaction
protected:
	virtual bool ProcessInteraction_Implementation(AActor* TargetActor) override;

	// Auto close
protected:
	/**
	 * Pause duration after which the door automatically starts closing.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(ClampMint="0"))
	float AutoCloseDelay = 2.f;

	UPROPERTY(BlueprintReadOnly, Category="Door")
	FTimerHandle AutoCloseDelayHandle;

	bool IsClosingAutomatically() const;
	
	virtual void StartAutoClose();
	
	virtual void StopAutoClose();
	
	// Key
private:
	/**
	 * The key class which is required to interact with the door.
	 * If none, the door does not require the key.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(AllowPrivateAccess="true", EditCondition="bRequireKey"))
	TSubclassOf<UKeyType> KeyClass = nullptr;

	bool HasKey(const AActor* Actor) const;
};
