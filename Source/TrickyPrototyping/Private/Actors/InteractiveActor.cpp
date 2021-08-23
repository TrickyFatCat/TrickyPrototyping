// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#include "Actors/InteractiveActor.h"
#include "Components/TimelineComponent.h"

DECLARE_LOG_CATEGORY_CLASS(LogInteractiveObject, All, All);

AInteractiveActor::AInteractiveActor()
{
	PrimaryActorTick.bCanEverTick = true;

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("AnimationTimeline");
}

void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	check(AnimationTimeline);
	CalculatePlayRate();

	if (AnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("AnimateTransform"));
		AnimationTimeline->AddInterpFloat(AnimationCurve, AnimationProgress);

		FOnTimelineEvent AnimationFinished;
		AnimationFinished.BindUFunction(this, FName("FinishAnimation"));
		AnimationTimeline->SetTimelineFinishedFunc(AnimationFinished);
	}

	if (AnimatedComponents.Num() > 0)
	{
		for (const auto Component : AnimatedComponents)
		{
			InitialTransforms.Add(Component->GetRelativeTransform());
		}
	}

	checkf(TargetTransforms.Num() != InitialTransforms.Num(),
	       TEXT("Check number of items in TargetTransforms in %s"),
	       *GetName());
}

void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveActor::SetAnimationDuration(const float Value)
{
	if (Value < 0.f) return;

	AnimationDuration = Value;
	CalculatePlayRate();
}

void AInteractiveActor::SetAnimationProgress(const float Progress)
{
	if (Progress < 0.f || Progress > 1.f) return;

	AnimateTransform(Progress);
}

void AInteractiveActor::StartAnimation()
{
	if (!AnimationCurve || GetAnimationDuration() <= 0.f) return;

	SetTargetState();

	switch (StateTarget)
	{
	case EInteractiveActorState::Opened:
		AnimationTimeline->PlayFromStart();
		break;
	case EInteractiveActorState::Closed:
		AnimationTimeline->ReverseFromEnd();
		break;
	}
}

void AInteractiveActor::ReverseAnimation()
{
	if (!AnimationCurve || !GetIsRevertible() || GetAnimationDuration() <= 0.f) return;

	StateTarget = StateTarget == EInteractiveActorState::Closed
		              ? EInteractiveActorState::Opened
		              : EInteractiveActorState::Closed;
	AnimationTimeline->Reverse();
}

void AInteractiveActor::StopAnimation()
{
	AnimationTimeline->Stop();
}

void AInteractiveActor::FinishAnimation()
{
	SetState(StateTarget);
}

void AInteractiveActor::CalculatePlayRate()
{
	if (!AnimationTimeline) return;

	AnimationTimeline->SetPlayRate(1.f / AnimationDuration);
}

void AInteractiveActor::AnimateTransform(const float AnimationProgress)
{
	for (int32 i = 0; i < AnimatedComponents.Num(); ++i)
	{
		FTransform NewTransform = InitialTransforms[i];
		FTransform TargetTransform = TargetTransforms[i];

		NewTransform.SetLocation(NewTransform.GetLocation() + TargetTransform.GetLocation() * AnimationProgress);

		FQuat NewRotation = FRotator(
				NewTransform.GetRotation().Rotator() + TargetTransform.GetRotation().Rotator() * AnimationProgress).
			Quaternion();
		NewTransform.SetRotation(NewRotation);

		NewTransform.SetScale3D(NewTransform.GetScale3D() + TargetTransform.GetScale3D() * AnimationProgress);

		AnimatedComponents.Array()[i]->SetRelativeTransform(NewTransform);
	}
}

bool AInteractiveActor::CanBeReversed() const
{
	return IsStateCurrent(EInteractiveActorState::Transition) && bIsRevertible;
}

void AInteractiveActor::SetState(const EInteractiveActorState NewState)
{
	if (!IsStateCurrent(NewState)) return;

	StatePrevious = StateCurrent;
	StateCurrent = NewState;
	OnActorChangedState.Broadcast(NewState);
	OnStateChanged(NewState);
}

void AInteractiveActor::SetTargetState()
{
	StateTarget = StateCurrent == EInteractiveActorState::Closed
		              ? EInteractiveActorState::Opened
		              : EInteractiveActorState::Closed;
}

bool AInteractiveActor::CanStartAnimation() const
{
	return !IsStateCurrent(EInteractiveActorState::Locked) || !IsStateCurrent(EInteractiveActorState::Disabled);
}

void AInteractiveActor::Open() 
{
	if (IsStateCurrent(EInteractiveActorState::Opened) || !CanStartAnimation()) return;

	if (CanBeReversed() && StateTarget == EInteractiveActorState::Closed)
	{
		ReverseAnimation();
		return;
	}

	if (AnimationDuration <= 0.f)
	{
		SetTargetState();
		AnimateTransform(1.f);
		FinishAnimation();
		return;
	}

	StartAnimation();
}

void AInteractiveActor::Close()
{
	if (IsStateCurrent(EInteractiveActorState::Closed) || !CanStartAnimation()) return;

	if (CanBeReversed() && StateTarget == EInteractiveActorState::Opened)
	{
		ReverseAnimation();
		return;
	}

	if (AnimationDuration <= 0.f)
	{
		SetTargetState();
		AnimateTransform(0.f);
		FinishAnimation();
		return;
	}

	StartAnimation();
}

void AInteractiveActor::Lock()
{
	if (!IsStateCurrent(EInteractiveActorState::Closed)) return;

	SetState(EInteractiveActorState::Locked);
}

void AInteractiveActor::Unlock()
{
	if (!IsStateCurrent(EInteractiveActorState::Locked)) return;

	SetState(EInteractiveActorState::Closed);
}

void AInteractiveActor::Enable()
{
	if (IsStateCurrent(EInteractiveActorState::Disabled)) return;

	SetState(StatePrevious);
}

void AInteractiveActor::Disable()
{
	if (!IsStateCurrent(EInteractiveActorState::Transition)) return;

	SetState(EInteractiveActorState::Disabled);
}
