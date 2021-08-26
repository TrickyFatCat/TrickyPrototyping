// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/TriggerComponents/BaseBoxTriggerComponent.h"
#include "InteractionBoxComponent.generated.h"

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UInteractionBoxComponent : public UBaseBoxTriggerComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Trigger")
	bool IsNormalTrigger() const { return bUseAsNormalTrigger; }

private:
	UPROPERTY(EditDefaultsOnly, Category="Trigger", meta=(AllowPrivateAccess="true"))
	bool bUseAsNormalTrigger = false;

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
