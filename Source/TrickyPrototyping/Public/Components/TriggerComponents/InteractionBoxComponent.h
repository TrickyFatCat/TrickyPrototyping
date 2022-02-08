// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/TriggerComponents/BaseBoxTriggerComponent.h"
#include "InteractionBoxComponent.generated.h"

/**
 * A box trigger which adds an actor to an interaction queue
 */
UCLASS(ClassGroup=(TrickyPrototyping), BlueprintType, meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UInteractionBoxComponent : public UBaseBoxTriggerComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintGetter, Category="TriggerComponent|Interaction")
	bool GetIsNormalTrigger() const { return bIsNormalTrigger; }

	UFUNCTION(BlueprintSetter, Category="TriggerComponent|Interaction")
	void SetIsNormalTrigger(const bool Value) { bIsNormalTrigger = Value; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TriggerComponent|Interaction")
	bool bRequireLineOfSight = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TriggerComponent|Interaction")
	FString InteractionMessage = "";
	
private:
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetIsNormalTrigger,
		BlueprintSetter=SetIsNormalTrigger,
		Category="TriggerComponent|Interaction",
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
