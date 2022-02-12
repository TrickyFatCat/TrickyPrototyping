// Created by Artyom Volkov during the UE4 course


#include "Actors/AnimatedActors/DoorSwing.h"
#include "Components/TriggerComponents/InteractionBoxComponent.h"

ADoorSwing::ADoorSwing()
{
}

bool ADoorSwing::ProcessInteraction_Implementation(AActor* TargetActor)
{
	if (!TargetActor) return false;

	if (IsStateCurrent(EAnimatedActorState::Closed))
	{
		CalculateTransformOffsets(TargetActor);
	}

	return Super::ProcessInteraction_Implementation(TargetActor);
}

void ADoorSwing::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (GetDoorType() == EDoorType::Auto && IsStateCurrent(EAnimatedActorState::Closed))
	{
		CalculateTransformOffsets(OtherActor);
	}

	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoorSwing::CalculateTransformOffsets(const AActor* Actor)
{
	if (TransformOffsets.Num() == 0 || !Actor || GetDoorType() == EDoorType::Manual) return;

	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal());
	PrevSwingDirection = SwingDirection;
	SwingDirection = FMath::Sign(DotProduct);

	if (PrevSwingDirection == SwingDirection) return;

	const FRotator FirstRotator = TransformOffsets[0].GetRotation().Rotator();
	auto IsAxisValid = [&](const float Axis) { return (SwingDirection != FMath::Sign(Axis) && Axis != 0); };

	for (FTransform& Offset : TransformOffsets)
	{
		if (IsAxisValid(FirstRotator.Roll)) continue;
		
		if (IsAxisValid(FirstRotator.Pitch)) continue;
		
		if (IsAxisValid(FirstRotator.Yaw)) continue;

		FRotator NewRotator{Offset.GetRotation()};
		NewRotator.Roll *= -1;
		NewRotator.Pitch *= -1;
		NewRotator.Yaw *= -1;
		Offset.SetRotation(NewRotator.Quaternion());
	}
}
