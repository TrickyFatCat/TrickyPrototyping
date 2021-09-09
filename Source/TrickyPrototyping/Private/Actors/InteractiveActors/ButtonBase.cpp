// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/InteractiveActors/ButtonBase.h"
#include "Components/TriggerComponents/InteractionCapsuleComponent.h"
#include "Components/SceneComponent.h"

AButtonBase::AButtonBase()
{
	ButtonRoot = CreateDefaultSubobject<USceneComponent>("ButtonRoot");
	SetRootComponent(ButtonRoot);

	ButtonTrigger = CreateDefaultSubobject<UInteractionCapsuleComponent>("ButtonTrigger");
	ButtonTrigger->SetupAttachment(ButtonRoot);
}

void AButtonBase::BeginPlay()
{
	Super::BeginPlay();

	ButtonTrigger->SetIsNormalTrigger(!bRequireInteraction);
}

void AButtonBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AButtonBase::Disable()
{
	Super::Disable();

	ButtonTrigger->SetIsEnabled(false);
}

void AButtonBase::Enable()
{
	if (bPressOnce) return;
	
	Super::Enable();
	ButtonTrigger->SetIsEnabled(true);
}

void AButtonBase::StartAnimation()
{
	Super::StartAnimation();

	ButtonTrigger->SetIsEnabled(false);
}

void AButtonBase::FinishAnimation()
{
	Super::FinishAnimation();

	if (bPressOnce)
	{
		Disable();
		return;
	}

	switch (ButtonBehaviour)
	{
	case EButtonBehaviour::Key:
		if (IsStateCurrent(EInteractiveActorState::Closed))
		{
			if (!GetWorld()) return;

			GetWorld()->GetTimerManager().SetTimer(AutoCloseDelayHandle,
			                                       this,
			                                       &AButtonBase::Close,
			                                       KeyAutoCloseDelayDuration,
			                                       false);
		}
		else
		{
			ButtonTrigger->SetIsEnabled(true);
		}
		break;

	case EButtonBehaviour::Switch:
		ButtonTrigger->SetIsEnabled(true);
		break;
	}
}
