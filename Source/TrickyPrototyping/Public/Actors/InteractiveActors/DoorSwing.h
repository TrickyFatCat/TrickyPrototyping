// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActors/DoorBase.h"
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
	virtual void BeginPlay() override;

	virtual void Close() override;

	float SwingDirection = 1.f;

	float PrevSwingDirection = 1.f;

	TArray<FInteractiveActorData> DefaultOffsets;

	virtual bool ProcessInteraction_Implementation(AActor* TargetActor) override;

	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp,
	                                   int32 OtherBodyIndex,
	                                   bool bFromSweep,
	                                   const FHitResult& SweepResult) override;

	virtual void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                                 AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp,
	                                 int32 OtherBodyIndex) override;

	void CalculateTargetTransform(const AActor* Actor);
};
