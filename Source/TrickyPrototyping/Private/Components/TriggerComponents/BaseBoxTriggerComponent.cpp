// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/BaseBoxTriggerComponent.h"

UBaseBoxTriggerComponent::UBaseBoxTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UBaseBoxTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UBaseBoxTriggerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBaseBoxTriggerComponent::OnEndOverlap);
	bIsEnabled ? EnableTrigger_Implementation() : DisableTrigger_Implementation();
}

void UBaseBoxTriggerComponent::TickComponent(float DeltaTime,
                                             ELevelTick Tick,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);
}

void UBaseBoxTriggerComponent::SetIsEnabled(const bool Value)
{
	if (bIsEnabled == Value) return;

	bIsEnabled = Value;
	bIsEnabled ? EnableTrigger_Implementation() : DisableTrigger_Implementation();
}

void UBaseBoxTriggerComponent::EnableTrigger_Implementation()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBaseBoxTriggerComponent::DisableTrigger_Implementation()
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
	bIsActorInside = true;
}

void UBaseBoxTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                            AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp,
                                            int32 OtherBodyIndex)
{
	bIsActorInside = false;
}
