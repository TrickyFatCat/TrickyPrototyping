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

	SortQueue();
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

void UInteractionQueueComponent::SortQueue()
{
	if (InteractionQueue.Num() <= 1 || !bSortByLineOfSight) return;

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

	if (InteractionQueue.Contains(HitResult.Actor))
	{
		UE_LOG(LogTemp, Error, TEXT("Target Actor: %s"), *HitResult.Actor->GetName());
		InteractionQueue.Remove(HitResult.GetActor());
		InteractionQueue.Insert(HitResult.GetActor(), 0);
	}
}
