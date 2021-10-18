// Created by Artyom Volkov during the UE4 course


#include "Actors/InteractiveActors/DoorSwing.h"

ADoorSwing::ADoorSwing()
{
}

void ADoorSwing::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorSwing::Close()
{
	SwingDirection *= -1;
	Super::Close();
}

bool ADoorSwing::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	const AActor* PlayerActor = Cast<AActor>(PlayerController->GetPawn());

	if (!PlayerActor) return false;

	CalculateTargetTransform(PlayerActor);
	return Super::ProcessInteraction_Implementation(PlayerController);
}

void ADoorSwing::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (GetDoorType() == EDoorType::Interactive)
	{
		CalculateTargetTransform(OtherActor);
	}

	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoorSwing::CalculateTargetTransform(const AActor* Actor)
{
	if (TransformOffsets.Num() == 0 || !Actor) return;

	for (int32 i = 0; i < TransformOffsets.Num(); ++i)
	{
		PrevSwingDirection = SwingDirection;
		const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
													 (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal());
		SwingDirection = FMath::Sign(DotProduct);

		const float CurrentYaw = TransformOffsets[i].RotationOffset.Yaw;

		if (PrevSwingDirection != SwingDirection)
		{
			PrevSwingDirection = SwingDirection;;
			return;
		}

		const float CurrentYawSign = FMath::Sign(CurrentYaw);

		if (SwingDirection < 0.f && CurrentYawSign < 0.f)
		{
			TransformOffsets[i].RotationOffset.Yaw *= SwingDirection;
		}
		else if (SwingDirection > 0.f && CurrentYawSign > 0.f)
		{
			TransformOffsets[i].RotationOffset.Yaw *= -SwingDirection;
		}
	}
}
