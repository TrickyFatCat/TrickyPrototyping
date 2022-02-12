// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#include "Actors/AnimatedActor.h"
#include "Components/TimelineComponent.h"

DECLARE_LOG_CATEGORY_CLASS(LogAnimatedActor, All, All);

AAnimatedActor::AAnimatedActor()
{
	PrimaryActorTick.bCanEverTick = true;

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("AnimationTimeline");
}

void AAnimatedActor::BeginPlay()
{
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

	if (TransformOffsets.Num() != InitialTransforms.Num())
	{
		UE_LOG(LogAnimatedActor,
		       Error,
		       TEXT("Check Target Transforms in %s. Target transforms length %i | Initial transforms length %i"),
		       *GetName(),
		       TransformOffsets.Num(),
		       InitialTransforms.Num());
		return;
	}
	
	Super::BeginPlay();
}

void AAnimatedActor::SetAnimationDuration(const float Value)
{
	if (Value <= 0.f) return;

	AnimationDuration = Value;
	CalculatePlayRate();
}

void AAnimatedActor::AddAnimatedComponent(USceneComponent* NewComponent)
{
	if (AnimatedComponents.Num() > 0)
	{
		AnimatedComponents.Empty();
	}

	if (AnimatedComponents.Contains(NewComponent) || !NewComponent) return;

	AnimatedComponents.AddUnique(NewComponent);
	FillInitialTransforms();
	SetInitialTransform();
}

void AAnimatedActor::FillAnimatedComponents(TArray<USceneComponent*> Components)
{
	if (Components.Num() == 0) return;

	AnimatedComponents.Empty();

	for (const auto Component : Components)
	{
		if (!Component || AnimatedComponents.Contains(Component)) continue;

		AnimatedComponents.AddUnique(Component);
	}

	FillInitialTransforms();
	SetInitialTransform();
}

void AAnimatedActor::FillInitialTransforms()
{
	if (AnimatedComponents.Num() <= 0) return;

	InitialTransforms.Empty();
	
	for (const auto Component : AnimatedComponents)
	{
		if (!Component) continue;

		InitialTransforms.Add(Component->GetRelativeTransform());
	}
}

void AAnimatedActor::SetInitialTransform()
{
	if (AnimatedComponents.Num() == 0) return;

	switch (StateInitial)
	{
		case EAnimatedActorState::Opened:
			SetState(EAnimatedActorState::Opened);
			AnimateTransform(1.f);
			break;

		case EAnimatedActorState::Closed:
			SetState(EAnimatedActorState::Closed);
			AnimateTransform(0.f);
			break;

		case EAnimatedActorState::Locked:
			SetState(EAnimatedActorState::Locked);
			AnimateTransform(0.f);
			break;

		case EAnimatedActorState::Disabled:
			Disable();
			break;

		default:
			break;
	}
}

void AAnimatedActor::StartAnimation()
{
	if (TransformOffsets.Num() != InitialTransforms.Num())
	{
		UE_LOG(LogAnimatedActor,
		       Error,
		       TEXT("Check Target Transforms in %s. Target transforms length %i | Initial transforms length %i"),
		       *GetName(),
		       TransformOffsets.Num(),
		       InitialTransforms.Num());
		return;
	}

	SetTargetState();
	SetState(EAnimatedActorState::Transition);
	OnActorTransitionStarted.Broadcast(StateTarget);
	OnTransitionStarted();

	switch (StateTarget)
	{
		case EAnimatedActorState::Opened:
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
		case EAnimatedActorState::Closed:
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

void AAnimatedActor::ReverseAnimation()
{
	if (!AnimationCurve || !bIsReversible || GetAnimationDuration() <= 0.f) return;

	StateTarget = StateTarget == EAnimatedActorState::Closed
		              ? EAnimatedActorState::Opened
		              : EAnimatedActorState::Closed;

	switch (StateTarget)
	{
		case EAnimatedActorState::Opened:
			AnimationTimeline->Play();
			break;
		case EAnimatedActorState::Closed:
			AnimationTimeline->Reverse();
			break;
	}

	OnActorTransitionReversed.Broadcast(StateTarget);
	OnTransitionReversed();
}

void AAnimatedActor::StopAnimation()
{
	AnimationTimeline->Stop();
}

void AAnimatedActor::FinishAnimation_Implementation()
{
	SetState(StateTarget);
}

void AAnimatedActor::CalculatePlayRate()
{
	if (!AnimationTimeline || !AnimationCurve) return;

	float MinTime, MaxTime;
	AnimationCurve->GetTimeRange(MinTime, MaxTime);

	AnimationTimeline->SetPlayRate(MaxTime / AnimationDuration);
}

void AAnimatedActor::AnimateTransform(const float AnimationProgress)
{
	if (InitialTransforms.Num() == 0 || TransformOffsets.Num() == 0 || InitialTransforms.Num() != TransformOffsets.Num()) return; // TODO Print error

	for (int32 i = 0; i < AnimatedComponents.Num(); ++i)
	{
		FTransform NewTransform = InitialTransforms[i];
		FTransform TargetTransform;
		TargetTransform.SetLocation(TransformOffsets[i].GetLocation());
		TargetTransform.SetRotation(TransformOffsets[i].GetRotation());
		TargetTransform.SetScale3D(TransformOffsets[i].GetScale3D());

		if (TargetTransform.GetLocation() != FVector::ZeroVector)
		{
			NewTransform.SetLocation(NewTransform.GetLocation() + TargetTransform.GetLocation() * AnimationProgress);
		}

		if (TargetTransform.GetRotation() != FRotator::ZeroRotator.Quaternion())
		{
			FQuat NewRotation{
				FRotator(NewTransform.GetRotation().Rotator() + TargetTransform.GetRotation().Rotator() *
				         AnimationProgress).Quaternion()
			};
			NewTransform.SetRotation(NewRotation);
		}

		if (TargetTransform.GetScale3D() != FVector::ZeroVector)
		{
			NewTransform.SetScale3D(NewTransform.GetScale3D() + TargetTransform.GetScale3D() * AnimationProgress);
		}

		AnimatedComponents[i]->SetRelativeTransform(NewTransform);
	}
}

bool AAnimatedActor::CanBeReversed() const
{
	return IsStateCurrent(EAnimatedActorState::Transition) && bIsReversible;
}

void AAnimatedActor::SetState(const EAnimatedActorState NewState)
{
	if (IsStateCurrent(NewState)) return;

	StatePrevious = StateCurrent;
	StateCurrent = NewState;
	OnActorChangedState.Broadcast(NewState);
	OnStateChanged(NewState);
}

void AAnimatedActor::SetTargetState()
{
	StateTarget = StateCurrent == EAnimatedActorState::Closed
		              ? EAnimatedActorState::Opened
		              : EAnimatedActorState::Closed;
}

bool AAnimatedActor::CanStartAnimation() const
{
	return !IsStateCurrent(EAnimatedActorState::Locked) || !IsStateCurrent(EAnimatedActorState::Disabled);
}

void AAnimatedActor::Open_Implementation()
{
	if (IsStateCurrent(EAnimatedActorState::Opened) || !CanStartAnimation()) return;

	if (IsStateCurrent(EAnimatedActorState::Locked) || IsStateCurrent(EAnimatedActorState::Disabled)) return;

	if (CanBeReversed() && StateTarget == EAnimatedActorState::Closed)
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

void AAnimatedActor::Close_Implementation()
{
	if (IsStateCurrent(EAnimatedActorState::Closed) || !CanStartAnimation()) return;

	if (IsStateCurrent(EAnimatedActorState::Locked) || IsStateCurrent(EAnimatedActorState::Disabled)) return;

	if (CanBeReversed() && StateTarget == EAnimatedActorState::Opened)
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

void AAnimatedActor::Lock_Implementation()
{
	if (IsStateCurrent(EAnimatedActorState::Closed) || IsStateCurrent(EAnimatedActorState::Opened))
	{
		SetState(EAnimatedActorState::Locked);
	}
}

void AAnimatedActor::Unlock_Implementation()
{
	if (!IsStateCurrent(EAnimatedActorState::Locked)) return;

	SetState(StatePrevious);
}

void AAnimatedActor::Enable_Implementation()
{
	if (IsStateCurrent(EAnimatedActorState::Disabled)) return;

	SetState(StatePrevious);
}

void AAnimatedActor::Disable_Implementation()
{
	if (!IsStateCurrent(EAnimatedActorState::Transition)) return;

	SetState(EAnimatedActorState::Disabled);
}
