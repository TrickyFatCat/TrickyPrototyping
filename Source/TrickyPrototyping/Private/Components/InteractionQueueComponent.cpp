// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/InteractionQueueComponent.h"

#include "Core/TrickyUtils.h"
#include "Interfaces/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UInteractionQueueComponent::AddToQueue(AActor* Actor,
                                            const bool bRequireLineOfSight,
                                            const FString& InteractionMessage)
{
	if (!IsValid(Actor) || QueueContainsActor(Actor)) return;

	const FInteractionData NewData{Actor, bRequireLineOfSight, InteractionMessage};
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

	return IInteractionInterface::Execute_ProcessInteraction(TargetActor, GetOwner());
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

FInteractionData UInteractionQueueComponent::GetFirstDataInQueue() const
{
	if (IsQueueEmpty()) return FInteractionData();

	return InteractionQueue[0];
}

FString UInteractionQueueComponent::GetInteractionMessage() const
{
	if (InteractionQueue.Num() == 0) return "";

	return InteractionQueue[0].InteractionMessage;
}

void UInteractionQueueComponent::CheckLineOfSight()
{
	if (IsQueueEmpty()) return;

	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	if (!FTrickyUtils::GetPlayerViewPoint(GetOwner(), ViewLocation, ViewRotation)) return;

	FVector TraceStart = ViewLocation;
	FVector TraceDirection = ViewRotation.Vector();
	FVector TraceEnd = TraceStart + TraceDirection * SightDistance;

	if (!GetWorld()) return;

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
	                                        TraceStart,
	                                        TraceEnd,
	                                        SightRadius,
	                                        UEngineTypes::ConvertToTraceType(ECC_Visibility),
	                                        false,
	                                        {GetOwner()},
	                                        EDrawDebugTrace::None,
	                                        HitResult,
	                                        true);

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
	                                                                     [&](const FInteractionData& Data)
	                                                                     {
		                                                                     return Data.Actor == Actor;
	                                                                     });
	return FinalData.Actor;
}

int32 UInteractionQueueComponent::GetInteractionDataIndex(const AActor* Actor) const
{
	return InteractionQueue.IndexOfByPredicate(
	                                           [&](const FInteractionData& Data) { return Data.Actor == Actor; });
}
