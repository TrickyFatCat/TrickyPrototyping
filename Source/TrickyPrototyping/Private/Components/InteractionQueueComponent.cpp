// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/InteractionQueueComponent.h"

#include "Core/TrickyUtils.h"
#include "Interfaces/InteractionInterface.h"

UInteractionQueueComponent::UInteractionQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionQueueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionQueueComponent::TickComponent(float DeltaTime,
                                               ELevelTick Tick,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	CheckLineOfSight();
}

void UInteractionQueueComponent::AddToQue(AActor* Actor, bool bRequireLineOfSight)
{
	if (!IsValid(Actor) || QueueContainsActor(Actor)) return;

	FInteractionData NewData;
	NewData.Actor = Actor;
	NewData.bRequireLineOfSight = bRequireLineOfSight;
	InteractionQueue.Add(NewData);
}

void UInteractionQueueComponent::RemoveFromQueue(const AActor* Actor)
{
	if (!IsValid(Actor) || !QueueContainsActor(Actor)) return;

	InteractionQueue.RemoveAt(GetInteractionDataIndex(Actor));
}

bool UInteractionQueueComponent::Interact()
{
	if (IsQueueEmpty()) return false;

	AActor* TargetActor = GetFirstActorInQueue();

	if (!IsValid(TargetActor)) return false;

	if (InteractionQueue[0].bRequireLineOfSight && TargetActor != ActorInSight) return false;

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());

	if (!PlayerController) return false;

	return IInteractionInterface::Execute_ProcessInteraction(TargetActor, PlayerController);
}

AActor* UInteractionQueueComponent::GetFirstActorInQueue() const
{
	if (IsQueueEmpty()) return nullptr;

	AActor* TargetActor = InteractionQueue[0].Actor;

	while (!IsValid(TargetActor))
	{
		TargetActor = InteractionQueue[0].Actor;
	}

	return TargetActor;
}

void UInteractionQueueComponent::CheckLineOfSight()
{
	if (IsQueueEmpty()) return;

	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	if (!FTrickyUtils::GetPlayerViewPoint(GetOwner(), ViewLocation, ViewRotation)) return;

	FVector TraceStart = ViewLocation;
	FVector TraceDirection = ViewRotation.Vector();
	FVector TraceEnd = TraceStart + TraceDirection * SortDistance;

	if (!GetWorld()) return;

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     TraceStart,
	                                     TraceEnd,
	                                     ECollisionChannel::ECC_Visibility,
	                                     CollisionQueryParams);

	ActorInSight = HitResult.GetActor();

	SortQueueByLineOfSight(HitResult);
}

void UInteractionQueueComponent::SortQueueByLineOfSight(FHitResult& HitResult)
{
	if (!bSortByLineOfSight || !HitResult.GetActor()) return;

	if (QueueContainsActor(HitResult.GetActor()))
	{
		const int32 Index = GetInteractionDataIndex(HitResult.GetActor());
		const FInteractionData Data = InteractionQueue[Index];
		InteractionQueue.RemoveAt(Index);
		InteractionQueue.Insert(Data, 0);
	}
}

bool UInteractionQueueComponent::QueueContainsActor(const AActor* Actor) const
{
	const auto bContains = InteractionQueue.FindByPredicate([&](const FInteractionData& Data)
	{
		return Data.Actor == Actor;
	});
	return bContains != nullptr;
}

AActor* UInteractionQueueComponent::GetQueuedActor(const AActor* Actor) const
{
	const FInteractionData FinalData = *InteractionQueue.FindByPredicate(
		[&](const FInteractionData& Data) { return Data.Actor == Actor; });
	return FinalData.Actor;
}

int32 UInteractionQueueComponent::GetInteractionDataIndex(const AActor* Actor) const
{
	return InteractionQueue.IndexOfByPredicate(
		[&](const FInteractionData& Data) { return Data.Actor == Actor; });
}
