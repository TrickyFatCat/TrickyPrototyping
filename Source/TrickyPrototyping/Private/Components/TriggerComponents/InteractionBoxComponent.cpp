// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/InteractionBoxComponent.h"
#include "Components/InteractionQueManagerComponent.h"

void UInteractionBoxComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                             AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex,
                                             bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bUseAsNormalTrigger) return;

	UInteractionQueManagerComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueManagerComponent>();

	if (!InteractionQue) return;

	InteractionQue->AddToQue(GetOwner());
}

void UInteractionBoxComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner()) || bUseAsNormalTrigger) return;

	UInteractionQueManagerComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueManagerComponent>();

	if (!InteractionQue) return;

	InteractionQue->RemoveFromQueue(GetOwner());
}
