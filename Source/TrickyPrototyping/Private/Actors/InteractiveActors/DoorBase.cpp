// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/InteractiveActors/DoorBase.h"
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
	if (AutoCloseDelay > 0.f && GetStateCurrent() == EInteractiveActorState::Opened)
	{
		StopAutoClose();
		return;
	}
	
	switch (DoorType)
	{
	case EDoorType::Auto:
		if (GetStateCurrent() != EInteractiveActorState::Closed) return;
		
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
	if (AutoCloseDelay > 0.f && IsStateCurrent(EInteractiveActorState::Opened))
	{
		StartAutoClose();
		return;
	}
	
	switch (DoorType)
	{
	case EDoorType::Auto:
		if (GetStateCurrent() != EInteractiveActorState::Opened) return;
		
		Close();
		break;

	case EDoorType::Interactive:
		break;
	}
}

bool ADoorBase::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController || DoorType != EDoorType::Interactive) return false;

	if (!GetIsReversible() && GetStateCurrent() == EInteractiveActorState::Transition) return false;

	StopAutoClose();
	StartAnimation();
	return true;
}

void ADoorBase::StartAutoClose()
{
	GetWorldTimerManager().SetTimer(AutoCloseDelayHandle, this, &ADoorBase::ProcessAutoClose, AutoCloseDelay, false);
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
