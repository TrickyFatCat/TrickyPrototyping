// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/InteractiveCapsuleComponent.h"
#include "Components/InteractionQueueComponent.h"

void UInteractiveCapsuleComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                  AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex,
                                                  bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bIsNormalTrigger) return;

	UInteractionQueueComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQue) return;

	InteractionQue->AddToQue(GetOwner());
}

void UInteractiveCapsuleComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bIsNormalTrigger) return;

	UInteractionQueueComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQue) return;

	InteractionQue->RemoveFromQueue(GetOwner());
}
