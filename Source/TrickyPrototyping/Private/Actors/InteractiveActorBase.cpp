// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#include "Actors/InteractiveActorBase.h"
#include "Components/TimelineComponent.h"

DECLARE_LOG_CATEGORY_CLASS(LogInteractiveActor, All, All);

AInteractiveActorBase::AInteractiveActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("AnimationTimeline");
}

void AInteractiveActorBase::BeginPlay()
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
	else
	{
		AnimationDuration = 0.f;
	}

	if (AnimatedComponents.Num() > 0)
	{
		for (const auto Component : AnimatedComponents)
		{
			if (!Component) continue;

			InitialTransforms.Add(Component->GetRelativeTransform());
		}
	}

	if (TargetTransforms.Num() > 0)
	{
		for (int32 i = 0; i < TargetTransforms.Num(); ++i)
		{
			if (!TargetTransforms[i].bAnimateScale) continue;

			FVector DeltaScale = InitialTransforms[i].GetScale3D() - TargetTransforms[i].TargetScale;
			DeltaScale.X = FMath::Abs(DeltaScale.X);
			DeltaScale.Y = FMath::Abs(DeltaScale.Y);
			DeltaScale.Z = FMath::Abs(DeltaScale.Z);
			TargetTransforms[i].TargetScale = DeltaScale;
		}
	}

	if (TargetTransforms.Num() != InitialTransforms.Num())
	{
		UE_LOG(LogInteractiveActor,
		       Error,
		       TEXT("Check Target Transforms in %s. Target transforms length %i | Initial transforms length %i"),
		       *GetName(),
		       TargetTransforms.Num(),
		       InitialTransforms.Num());
		return;
	}

	switch (StateInitial)
	{
	case EInteractiveActorState::Opened:
		SetState(EInteractiveActorState::Opened);
		AnimateTransform(1.f);
		break;

	case EInteractiveActorState::Closed:
		SetState(EInteractiveActorState::Closed);
		AnimateTransform(0.f);
		break;

	case EInteractiveActorState::Locked:
		SetState(EInteractiveActorState::Locked);
		AnimateTransform(0.f);
		break;

	case EInteractiveActorState::Disabled:
		Disable();
		break;

	default:
		break;
	}
}

void AInteractiveActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveActorBase::SetAnimationDuration(const float Value)
{
	if (Value <= 0.f) return;

	AnimationDuration = Value;
	CalculatePlayRate();
}

void AInteractiveActorBase::AddAnimatedComponent(USceneComponent* NewComponent)
{
	AnimatedComponents.Empty();

	if (AnimatedComponents.Contains(NewComponent) || !NewComponent) return;

	AnimatedComponents.AddUnique(NewComponent);
}

void AInteractiveActorBase::FillAnimatedComponents(TArray<USceneComponent*> Components)
{
	if (Components.Num() == 0) return;

	AnimatedComponents.Empty();

	for (const auto Component : Components)
	{
		if (!Component || AnimatedComponents.Contains(Component)) continue;

		AnimatedComponents.AddUnique(Component);
	}
}

void AInteractiveActorBase::StartAnimation()
{
	if (TargetTransforms.Num() != InitialTransforms.Num())
	{
		UE_LOG(LogInteractiveActor,
		       Error,
		       TEXT("Check Target Transforms in %s. Target transforms length %i | Initial transforms length %i"),
		       *GetName(),
		       TargetTransforms.Num(),
		       InitialTransforms.Num());
		return;
	}
	
	SetTargetState();
	SetState(EInteractiveActorState::Transition);
	OnActorTransitionStarted.Broadcast(StateTarget);
	OnTransitionStarted();

	switch (StateTarget)
	{
	case EInteractiveActorState::Opened:
		if (GetAnimationDuration() > 0.f)
		{
			AnimationTimeline->PlayFromStart();
		}
		else
		{
			AnimateTransform(1.0);
			FinishAnimation();
		}
		break;
	case EInteractiveActorState::Closed:
		if (GetAnimationDuration() > 0.f)
		{
			AnimationTimeline->ReverseFromEnd();
		}
		else
		{
			AnimateTransform(0.f);
			FinishAnimation();
		}
		break;
	}
}

void AInteractiveActorBase::ReverseAnimation()
{
	if (!AnimationCurve || !bIsReversible || GetAnimationDuration() <= 0.f) return;

	StateTarget = StateTarget == EInteractiveActorState::Closed
		              ? EInteractiveActorState::Opened
		              : EInteractiveActorState::Closed;

	switch (StateTarget)
	{
	case EInteractiveActorState::Opened:
		AnimationTimeline->Play();
		break;
	case EInteractiveActorState::Closed:
		AnimationTimeline->Reverse();
		break;
	}

	OnActorTransitionReversed.Broadcast(StateTarget);
	OnTransitionReversed();
}

void AInteractiveActorBase::StopAnimation()
{
	AnimationTimeline->Stop();
}

void AInteractiveActorBase::FinishAnimation()
{
	SetState(StateTarget);
}

void AInteractiveActorBase::CalculatePlayRate()
{
	if (!AnimationTimeline) return;

	AnimationTimeline->SetPlayRate(1.f / AnimationDuration);
}

void AInteractiveActorBase::AnimateTransform(const float AnimationProgress)
{
	for (int32 i = 0; i < AnimatedComponents.Num(); ++i)
	{
		FTransform NewTransform = InitialTransforms[i];
		FTransform TargetTransform;
		TargetTransform.SetLocation(TargetTransforms[i].TargetLocation);
		TargetTransform.SetRotation(TargetTransforms[i].TargetRotation.Quaternion());
		TargetTransform.SetScale3D(TargetTransforms[i].TargetScale);

		if (TargetTransforms[i].bAnimateLocation)
		{
			NewTransform.SetLocation(NewTransform.GetLocation() + TargetTransform.GetLocation() * AnimationProgress);
		}

		if (TargetTransforms[i].bAnimateRotation)
		{
			FQuat NewRotation = FRotator(
					NewTransform.GetRotation().Rotator() + TargetTransform.GetRotation().Rotator() * AnimationProgress).
				Quaternion();
			NewTransform.SetRotation(NewRotation);
		}

		if (TargetTransforms[i].bAnimateScale)
		{
			NewTransform.SetScale3D(NewTransform.GetScale3D() + TargetTransform.GetScale3D() * AnimationProgress);
		}

		AnimatedComponents[i]->SetRelativeTransform(NewTransform);
	}
}

bool AInteractiveActorBase::CanBeReversed() const
{
	return IsStateCurrent(EInteractiveActorState::Transition) && bIsReversible;
}

void AInteractiveActorBase::SetState(const EInteractiveActorState NewState)
{
	if (IsStateCurrent(NewState)) return;

	StatePrevious = StateCurrent;
	StateCurrent = NewState;
	OnActorChangedState.Broadcast(NewState);
	OnStateChanged(NewState);
}

void AInteractiveActorBase::SetTargetState()
{
	StateTarget = StateCurrent == EInteractiveActorState::Closed
		              ? EInteractiveActorState::Opened
		              : EInteractiveActorState::Closed;
}

bool AInteractiveActorBase::CanStartAnimation() const
{
	return !IsStateCurrent(EInteractiveActorState::Locked) || !IsStateCurrent(EInteractiveActorState::Disabled);
}

void AInteractiveActorBase::Open()
{
	if (IsStateCurrent(EInteractiveActorState::Opened) || !CanStartAnimation()) return;

	if (IsStateCurrent(EInteractiveActorState::Locked) || IsStateCurrent(EInteractiveActorState::Disabled)) return;
	
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

void AInteractiveActorBase::Close()
{
	if (IsStateCurrent(EInteractiveActorState::Closed) || !CanStartAnimation()) return;

	if (IsStateCurrent(EInteractiveActorState::Locked) || IsStateCurrent(EInteractiveActorState::Disabled)) return;
	
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

void AInteractiveActorBase::Lock()
{
	if (IsStateCurrent(EInteractiveActorState::Closed) || IsStateCurrent(EInteractiveActorState::Opened))
	{
		SetState(EInteractiveActorState::Locked);
	}
}

void AInteractiveActorBase::Unlock()
{
	if (!IsStateCurrent(EInteractiveActorState::Locked)) return;

	SetState(StatePrevious);
}

void AInteractiveActorBase::Enable()
{
	if (IsStateCurrent(EInteractiveActorState::Disabled)) return;

	SetState(StatePrevious);
}

void AInteractiveActorBase::Disable()
{
	if (!IsStateCurrent(EInteractiveActorState::Transition)) return;

	SetState(EInteractiveActorState::Disabled);
}
