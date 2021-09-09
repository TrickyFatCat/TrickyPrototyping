// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/InteractionQueueComponent.h"
#include "Interfaces/InteractionInterface.h"

UInteractionQueueComponent::UInteractionQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionQueueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionQueueComponent::AddToQue(AActor* Actor)
{
	if (!IsValid(Actor) || InteractionQueue.Contains(Actor)) return;

	InteractionQueue.AddUnique(Actor);
}

void UInteractionQueueComponent::RemoveFromQueue(AActor* Actor)
{
	if (!IsValid(Actor) || !InteractionQueue.Contains(Actor)) return;

	InteractionQueue.Remove(Actor);
}

bool UInteractionQueueComponent::Interact()
{
	if (IsQueueEmpty()) return false;

	AActor* TargetActor = GetTargetActor();

	if (!IsValid(TargetActor)) return false;

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());

	if (!PlayerController) return false;

	return IInteractionInterface::Execute_ProcessInteraction(TargetActor, PlayerController);
}

AActor* UInteractionQueueComponent::GetTargetActor() const
{
	if (IsQueueEmpty()) return nullptr;

	AActor* TargetActor = InteractionQueue[0];

	while (!IsValid(TargetActor))
	{
		TargetActor = InteractionQueue[0];
	}

	return TargetActor;
}

