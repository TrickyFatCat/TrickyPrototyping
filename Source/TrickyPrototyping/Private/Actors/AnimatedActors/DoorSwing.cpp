// Created by Artyom Volkov during the UE4 course


#include "Actors/AnimatedActors/DoorSwing.h"
#include "Components/TriggerComponents/InteractionBoxComponent.h"

ADoorSwing::ADoorSwing()
{
}

void ADoorSwing::BeginPlay()
{
	Super::BeginPlay();
	DefaultOffsets = TransformOffsets;
}

bool ADoorSwing::ProcessInteraction_Implementation(AActor* TargetActor)
{
	if (!TargetActor) return false;

	if (IsStateCurrent(EAnimatedActorState::Closed))
	{
		CalculateTargetTransform(TargetActor);
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
		CalculateTargetTransform(OtherActor);
	}

	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoorSwing::CalculateTargetTransform(const AActor* Actor)
{
	if (TransformOffsets.Num() == 0 || !Actor || GetDoorType() == EDoorType::Manual) return;

	PrevSwingDirection = SwingDirection;
	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
												 (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal());
	SwingDirection = FMath::Sign(DotProduct);

	if (PrevSwingDirection == SwingDirection) return;

	for (FTransform& Offset : TransformOffsets)
	{
		FRotator NewRotator{Offset.GetRotation()};
		NewRotator.Yaw *= -1;
		Offset.SetRotation(NewRotator.Quaternion());
	}
}
