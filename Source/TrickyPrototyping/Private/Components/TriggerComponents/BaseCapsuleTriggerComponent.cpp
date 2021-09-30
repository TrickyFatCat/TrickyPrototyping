// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/BaseCapsuleTriggerComponent.h"

UBaseCapsuleTriggerComponent::UBaseCapsuleTriggerComponent()
{
}

void UBaseCapsuleTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseCapsuleTriggerComponent::TickComponent(float DeltaTime,
                                                 ELevelTick Tick,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	OnComponentBeginOverlap.AddDynamic(this, &UBaseCapsuleTriggerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBaseCapsuleTriggerComponent::OnEndOverlap);
}

void UBaseCapsuleTriggerComponent::SetIsEnabled(const bool bEnabled)
{
	if (bIsEnabled == bEnabled) return;

	bIsEnabled = bEnabled;
	bIsEnabled ? EnableTrigger() : DisableTrigger();
}

void UBaseCapsuleTriggerComponent::EnableTrigger()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBaseCapsuleTriggerComponent::DisableTrigger()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UBaseCapsuleTriggerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                  AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex,
                                                  bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	bIsActorInside = true;
}

void UBaseCapsuleTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex)
{
	bIsActorInside = false;
}
