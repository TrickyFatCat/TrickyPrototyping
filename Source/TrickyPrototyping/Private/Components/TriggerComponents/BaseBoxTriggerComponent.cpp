// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/BaseBoxTriggerComponent.h"

UBaseBoxTriggerComponent::UBaseBoxTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseBoxTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetGenerateOverlapEvents(true);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetIsEnabled(bIsEnabled);

	OnComponentBeginOverlap.AddDynamic(this, &UBaseBoxTriggerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBaseBoxTriggerComponent::OnEndOverlap);
}

void UBaseBoxTriggerComponent::TickComponent(float DeltaTime,
                                             ELevelTick Tick,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);
}

void UBaseBoxTriggerComponent::SetIsEnabled(const bool bEnabled)
{
	if (bIsEnabled == bEnabled) return;

	bIsEnabled = bEnabled;
	bIsEnabled ? EnableTrigger() : DisableTrigger();
}

void UBaseBoxTriggerComponent::EnableTrigger()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBaseBoxTriggerComponent::DisableTrigger()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UBaseBoxTriggerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
}

void UBaseBoxTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}
