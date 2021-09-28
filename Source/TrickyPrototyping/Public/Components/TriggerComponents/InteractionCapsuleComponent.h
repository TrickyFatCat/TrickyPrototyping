// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/TriggerComponents/BaseCapsuleTriggerComponent.h"
#include "InteractionCapsuleComponent.generated.h"

/**
 * A capsule component which adds an actor to an interaction queeu
 */
UCLASS()
class TRICKYPROTOTYPING_API UInteractionCapsuleComponent : public UBaseCapsuleTriggerComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintGetter, Category="Trigger")
	bool GetIsNormalTrigger() const { return bIsNormalTrigger; }

	UFUNCTION(BlueprintSetter, Category="Trigger")
	void SetIsNormalTrigger(const bool Value) { bIsNormalTrigger = Value; }
	
	bool bRequireLineOfSight = false;

private:
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetIsNormalTrigger,
		BlueprintSetter=SetIsNormalTrigger,
		Category="Trigger",
		meta=(AllowPrivateAccess="true"))
	bool bIsNormalTrigger = false;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                            AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex,
	                            bool bFromSweep,
	                            const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                          AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex) override;
};
