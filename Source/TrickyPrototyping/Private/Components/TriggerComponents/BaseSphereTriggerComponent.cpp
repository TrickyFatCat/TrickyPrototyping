// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/BaseSphereTriggerComponent.h"

UBaseSphereTriggerComponent::UBaseSphereTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseSphereTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetGenerateOverlapEvents(true);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetIsEnabled(bIsEnabled);

	OnComponentBeginOverlap.AddDynamic(this, &UBaseSphereTriggerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBaseSphereTriggerComponent::OnEndOverlap);
}

void UBaseSphereTriggerComponent::TickComponent(float DeltaTime,
                                                ELevelTick Tick,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);
}

void UBaseSphereTriggerComponent::SetIsEnabled(const bool bEnabled)
{
	if (bIsEnabled == bEnabled) return;

	bIsEnabled = bEnabled;
	bIsEnabled ? EnableTrigger() : DisableTrigger();
}

void UBaseSphereTriggerComponent::EnableTrigger()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBaseSphereTriggerComponent::DisableTrigger()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UBaseSphereTriggerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	bIsActorInside = true;
}

void UBaseSphereTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bIsActorInside = false;
}

