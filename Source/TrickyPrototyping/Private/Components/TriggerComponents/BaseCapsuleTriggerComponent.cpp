// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/BaseCapsuleTriggerComponent.h"

UBaseCapsuleTriggerComponent::UBaseCapsuleTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UBaseCapsuleTriggerComponent::BeginPlay()
{
	OnComponentBeginOverlap.AddDynamic(this, &UBaseCapsuleTriggerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBaseCapsuleTriggerComponent::OnEndOverlap);
	bIsEnabled ? EnableTrigger() : DisableTrigger();
	
	Super::BeginPlay();
}

void UBaseCapsuleTriggerComponent::SetIsEnabled(const bool Value)
{
	if (bIsEnabled == Value) return;

	bIsEnabled = Value;
	bIsEnabled ? EnableTrigger() : DisableTrigger();
}

void UBaseCapsuleTriggerComponent::EnableTrigger_Implementation()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBaseCapsuleTriggerComponent::DisableTrigger_Implementation()
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
