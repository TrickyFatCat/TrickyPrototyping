// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/SphereIntTriggerComponent.h"
#include "Components/InteractionQueManagerComponent.h"

void USphereIntTriggerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex,
                                                bool bFromSweep,
                                                const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner())) return;

	UInteractionQueManagerComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueManagerComponent>();

	if (!InteractionQue) return;

	InteractionQue->AddToQue(GetOwner());
}

void USphereIntTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                              AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp,
                                              int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner())) return;

	UInteractionQueManagerComponent* InteractionQue = OtherActor->FindComponentByClass<UInteractionQueManagerComponent>();

	if (!InteractionQue) return;

	InteractionQue->RemoveFromQueue(GetOwner());
}
