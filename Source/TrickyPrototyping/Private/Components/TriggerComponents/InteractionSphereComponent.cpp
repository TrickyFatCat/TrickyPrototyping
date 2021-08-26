// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/InteractionSphereComponent.h"
#include "Components/InteractionQueueComponent.h"

void UInteractionSphereComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex,
                                                bool bFromSweep,
                                                const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bUseAsNormalTrigger) return;

	UInteractionQueueComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQue) return;

	InteractionQue->AddToQue(GetOwner());
}

void UInteractionSphereComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                              AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp,
                                              int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bUseAsNormalTrigger) return;

	UInteractionQueueComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQue) return;

	InteractionQue->RemoveFromQueue(GetOwner());
}
