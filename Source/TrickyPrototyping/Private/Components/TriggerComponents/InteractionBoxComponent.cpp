// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/InteractionBoxComponent.h"
#include "Components/InteractionQueueComponent.h"

void UInteractionBoxComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                             AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex,
                                             bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bIsNormalTrigger) return;

	UInteractionQueueComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQue) return;

	InteractionQue->AddToQueue(GetOwner(), bRequireLineOfSight, InteractionMessage);
}

void UInteractionBoxComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bIsNormalTrigger) return;

	UInteractionQueueComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQue) return;

	InteractionQue->RemoveFromQueue(GetOwner());
}
