// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/TriggerComponents/BaseSphereTriggerComponent.h"
#include "InteractionSphereComponent.generated.h"

/**
 * A sphere trigger which adds actor to an interaction queue
 */
UCLASS()
class TRICKYPROTOTYPING_API UInteractionSphereComponent : public UBaseSphereTriggerComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintGetter, Category="Trigger")
	bool GetIsNormalTrigger() const { return bIsNormalTrigger; }

	UFUNCTION(BlueprintSetter, Category="Trigger")
	void SetIsNormalTrigger(const bool Value) { bIsNormalTrigger = Value; }

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
