// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActorBase.h"
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
class TRICKYPROTOTYPING_API ADoorBase : public AInteractiveActorBase, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADoorBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void Disable() override;

	virtual void Enable() override;

protected:
	virtual void FinishAnimation() override;

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

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* DoorRoot = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UInteractionBoxComponent* DoorTrigger = nullptr;

	// Interaction
protected:
	virtual bool ProcessInteraction_Implementation(AActor* TargetActor) override;

	// Auto close
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(ClampMint="0"))
	float AutoCloseDelay = 2.f;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AutoCloseDelayHandle;

	bool IsClosingAutomatically() const;
	
	virtual void StartAutoClose();
	
	virtual void StopAutoClose();
	
	// Key
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door|Keys", meta=(AllowPrivateAccess="true"))
	bool bRequireKey = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door|Keys", meta=(AllowPrivateAccess="true", EditCondition="bRequireKey"))
	TSubclassOf<UKeyType> DoorKey = nullptr;

	bool HasKey(const AActor* Actor) const;
};
