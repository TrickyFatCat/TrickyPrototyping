// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/InteractionCapsuleComponent.h"
#include "Components/InteractionQueueComponent.h"

void UInteractionCapsuleComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
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

	InteractionQue->AddToQue(GetOwner(), bRequireLineOfSight);
}

void UInteractionCapsuleComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
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
