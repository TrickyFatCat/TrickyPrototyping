// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/InteractionQueManagerComponent.h"

#include "Interfaces/InteractionInterface.h"

UInteractionQueManagerComponent::UInteractionQueManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionQueManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionQueManagerComponent::AddToQue(AActor* Actor)
{
	if (!IsValid(Actor) || InteractionQueue.Contains(Actor)) return;

	InteractionQueue.AddUnique(Actor);
}

void UInteractionQueManagerComponent::RemoveFromQueue(AActor* Actor)
{
	if (!IsValid(Actor) || InteractionQueue.Contains(Actor)) return;

	InteractionQueue.Remove(Actor);
}

bool UInteractionQueManagerComponent::Interact() const
{
	if (IsQueueEmpty()) return false;

	AActor* TargetActor = GetTargetActor();

	if (!IsValid(TargetActor)) return false;

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());

	if (!PlayerController) return false;

	return IInteractionInterface::Execute_ProcessInteraction(TargetActor, PlayerController);
}

AActor* UInteractionQueManagerComponent::GetTargetActor() const
{
	if (IsQueueEmpty()) return nullptr;

	AActor* TargetActor = InteractionQueue[0];

	while (!IsValid(TargetActor))
	{
		TargetActor = InteractionQueue[0];
	}

	return TargetActor;
}

