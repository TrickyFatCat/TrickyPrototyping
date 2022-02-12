// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/AnimatedActors/DoorBase.h"
#include "DoorSwing.generated.h"

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API ADoorSwing : public ADoorBase
{
	GENERATED_BODY()

public:
	ADoorSwing();
	
protected:
	int32 SwingDirection = 0.f;

	int32 PrevSwingDirection = 0.f;

	virtual bool ProcessInteraction_Implementation(AActor* TargetActor) override;

	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp,
	                                   int32 OtherBodyIndex,
	                                   bool bFromSweep,
	                                   const FHitResult& SweepResult) override;

	void CalculateTransformOffsets(const AActor* Actor);
};
