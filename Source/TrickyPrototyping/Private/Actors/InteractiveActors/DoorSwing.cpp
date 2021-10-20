// Created by Artyom Volkov during the UE4 course


#include "Actors/InteractiveActors/DoorSwing.h"

ADoorSwing::ADoorSwing()
{
}

void ADoorSwing::BeginPlay()
{
	Super::BeginPlay();
	DefaultOffsets = TransformOffsets;
}

void ADoorSwing::Close()
{
	Super::Close();
}

bool ADoorSwing::ProcessInteraction_Implementation(AActor* TargetActor)
{
	if (!TargetActor) return false;

	CalculateTargetTransform(TargetActor);
	return Super::ProcessInteraction_Implementation(TargetActor);
}

void ADoorSwing::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	CalculateTargetTransform(OtherActor);

	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoorSwing::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                     AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex)
{
	CalculateTargetTransform(OtherActor);
	Super::OnTriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ADoorSwing::CalculateTargetTransform(const AActor* Actor)
{
	if (TransformOffsets.Num() == 0 || !Actor || GetDoorType() == EDoorType::Manual) return;

	PrevSwingDirection = SwingDirection;

	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             Actor->GetActorForwardVector());
	SwingDirection = FMath::Sign(DotProduct);

	if (PrevSwingDirection != SwingDirection)
	{
		PrevSwingDirection = SwingDirection;
		return;
	}

	if (SwingDirection > 0)
	{
		TransformOffsets = DefaultOffsets;
		return;
	}

	for (int32 i = 0; i < TransformOffsets.Num(); ++i)
	{
		const float OffsetYawSign = FMath::Sign(TransformOffsets[i].RotationOffset.Yaw);

		if (OffsetYawSign == -FMath::Sign(DefaultOffsets[i].RotationOffset.Yaw)) continue;

		TransformOffsets[i].RotationOffset.Yaw *= -1;
	}
}
