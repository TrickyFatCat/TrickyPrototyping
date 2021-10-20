// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/InteractiveActors/DoorBase.h"

#include "Components/KeyRingComponent.h"
#include "Components/TriggerComponents/InteractionBoxComponent.h"

ADoorBase::ADoorBase()
{
	DoorTrigger = CreateDefaultSubobject<UInteractionBoxComponent>("DoorTrigger");
	SetRootComponent(DoorTrigger);
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
	SwitchDoorType();

	DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorBase::OnTriggerBeginOverlap);
	DoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorBase::OnTriggerEndOverlap);
}

void ADoorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADoorBase::Disable()
{
	Super::Disable();

	DoorTrigger->SetIsEnabled(false);
}

void ADoorBase::Enable()
{
	Super::Enable();

	SwitchDoorType();
}

void ADoorBase::FinishAnimation()
{
	Super::FinishAnimation();

	if (AutoCloseDelay <= 0.f || DoorTrigger->GetIsActorInside()) return;

	StartAutoClose();
}

void ADoorBase::SetDoorType(const EDoorType Value)
{
	if (DoorType == Value) return;

	DoorType = Value;
	SwitchDoorType();
}

void ADoorBase::SwitchDoorType() const
{
	switch (DoorType)
	{
	case EDoorType::Auto:
		DoorTrigger->SetIsNormalTrigger(true);
		break;

	case EDoorType::Interactive:
		DoorTrigger->SetIsNormalTrigger(false);
		DoorTrigger->bRequireLineOfSight = bRequireLineOfSight;
		break;

	case EDoorType::Manual:
		DoorTrigger->SetIsEnabled(false);
		break;
	}
}

void ADoorBase::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex,
                                      bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (AutoCloseDelay > 0.f && GetStateCurrent() == EInteractiveActorState::Opened && !GetIsReversible())
	{
		StopAutoClose();
		return;
	}
	
	switch (DoorType)
	{
	case EDoorType::Auto:
		if (bRequireKey && !HasKey(OtherActor)) return;
		
		if (GetStateCurrent() != EInteractiveActorState::Closed && !GetIsReversible()) return;
		
		Open();
		break;

	case EDoorType::Interactive:
		break;
	}
}

void ADoorBase::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                    AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex)
{
	if (AutoCloseDelay > 0.f && IsStateCurrent(EInteractiveActorState::Opened) && !GetIsReversible())
	{
		StartAutoClose();
		return;
	}
	
	switch (DoorType)
	{
	case EDoorType::Auto:
		if (GetStateCurrent() != EInteractiveActorState::Opened && !GetIsReversible()) return;
		
		Close();
		break;

	case EDoorType::Interactive:
		break;
	}
}

bool ADoorBase::ProcessInteraction_Implementation(AActor* TargetActor)
{
	if (!TargetActor || DoorType != EDoorType::Interactive) return false;
	
	if (bRequireKey && !HasKey(TargetActor)) return false;

	if (GetIsReversible() && GetStateCurrent() == EInteractiveActorState::Transition)
	{
		switch (GetStateTarget())
		{
		case EInteractiveActorState::Opened:
			Close();
			break;

		case EInteractiveActorState::Closed:
			Open();
			break;
		}
		
		return true;
	}

	StopAutoClose();
	
	switch (GetStateCurrent())
	{
	case EInteractiveActorState::Opened:
		Close();
		break;

	case EInteractiveActorState::Closed:
		Open();
		break;
	}
	
	return true;
}

void ADoorBase::StartAutoClose()
{
	if (GetIsReversible()) return;
	
	GetWorldTimerManager().SetTimer(AutoCloseDelayHandle, this, &ADoorBase::Close, AutoCloseDelay, false);
}

void ADoorBase::StopAutoClose()
{
	if (GetWorldTimerManager().IsTimerActive(AutoCloseDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(AutoCloseDelayHandle);
	}
}

void ADoorBase::ProcessAutoClose()
{
	Close();
}

bool ADoorBase::HasKey(const AActor* Actor) const
{
	if (!IsValid(Actor)) return false;

	UKeyRingComponent* KeyRingComponent = Actor->FindComponentByClass<UKeyRingComponent>();

	if (!KeyRingComponent) return false;

	return KeyRingComponent->HasKey(DoorKey);
}
