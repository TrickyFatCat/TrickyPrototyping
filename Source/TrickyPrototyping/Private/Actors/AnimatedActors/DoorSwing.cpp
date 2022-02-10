// Created by Artyom Volkov during the UE4 course


#include "Actors/AnimatedActors/DoorSwing.h"
#include "Components/TriggerComponents/InteractionBoxComponent.h"

ADoorSwing::ADoorSwing()
{
}

void ADoorSwing::Close_Implementation()
{
	PrevSwingDirection = SwingDirection;
	Super::Close_Implementation();
}

void ADoorSwing::BeginPlay()
{
	Super::BeginPlay();
	DefaultOffsets = TransformOffsets;
}

void ADoorSwing::FinishAnimation_Implementation()
{
	Super::FinishAnimation_Implementation();

	if (GetDoorTrigger()->GetIsActorInside())
	{
		CalculateTargetTransform(Initiator);
	}
}

bool ADoorSwing::ProcessInteraction_Implementation(AActor* TargetActor)
{
	Super::ProcessInteraction_Implementation(TargetActor);

	if (!TargetActor) return false;

	Initiator = TargetActor;
	CalculateTargetTransform(Initiator);

	return true;
}

void ADoorSwing::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	Initiator = OtherActor;
	
	// if (IsStateCurrent(EAnimatedActorState::Transition))
	// {
	// 	CalculateTargetTransform(Initiator);
	// }
	
	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoorSwing::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                     AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex)
{
	// if (IsStateCurrent(EAnimatedActorState::Transition))
	// {
	// 	CalculateTargetTransform(Initiator);
	// }

	Initiator = nullptr;
	Super::OnTriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ADoorSwing::CalculateTargetTransform(const AActor* Actor)
{
	if (TransformOffsets.Num() == 0 || !Actor || GetDoorType() == EDoorType::Manual) return;
	
	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             Actor->GetActorLocation());
	SwingDirection = FMath::Sign(DotProduct);
	
	// if (PrevSwingDirection != SwingDirection)
	// {
	// 	PrevSwingDirection = SwingDirection;
	// 	return;
	// }
	//
	// if (SwingDirection > 0)
	// {
	// 	TransformOffsets = DefaultOffsets;
	// 	return;
	// }
	//
	// for (int32 i = 0; i < TransformOffsets.Num(); ++i)
	// {
	// 	const float OffsetYawSign = FMath::Sign(TransformOffsets[i].RotationOffset.Yaw);
	//
	// 	if (OffsetYawSign == -FMath::Sign(DefaultOffsets[i].RotationOffset.Yaw)) continue;
	//
	// 	TransformOffsets[i].RotationOffset.Yaw *= -1;
	// }
}
