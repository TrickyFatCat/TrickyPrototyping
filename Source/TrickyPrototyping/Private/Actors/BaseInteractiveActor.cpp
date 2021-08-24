// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#include "Actors/BaseInteractiveActor.h"
#include "Components/TimelineComponent.h"

DECLARE_LOG_CATEGORY_CLASS(LogInteractiveObject, All, All);

ABaseInteractiveActor::ABaseInteractiveActor()
{
	PrimaryActorTick.bCanEverTick = true;

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("AnimationTimeline");
}

void ABaseInteractiveActor::BeginPlay()
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
			if (!Component) continue;

			InitialTransforms.Add(Component->GetRelativeTransform());
		}
	}

	if (TargetTransforms.Num() > 0)
	{
		for (int32 i = 0; i < TargetTransforms.Num(); ++i)
		{
			if (InitialTransforms[i].GetScale3D() == TargetTransforms[i].GetScale3D()) continue;
			
			FVector DeltaScale = InitialTransforms[i].GetScale3D() - TargetTransforms[i].GetScale3D();
			DeltaScale.X = FMath::Abs(DeltaScale.X);
			DeltaScale.Y = FMath::Abs(DeltaScale.Y);
			DeltaScale.Z = FMath::Abs(DeltaScale.Z);
			TargetTransforms[i].SetScale3D(DeltaScale);
		}
	}

	checkf(TargetTransforms.Num() == InitialTransforms.Num(),
	       TEXT("Check Target Transforms in %s. Target transforms length %i | Initial transforms length %i"),
	       *GetName(),
	       TargetTransforms.Num(),
	       InitialTransforms.Num());
}

void ABaseInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseInteractiveActor::SetAnimationDuration(const float Value)
{
	if (Value < 0.f) return;

	AnimationDuration = Value;
	CalculatePlayRate();
}

void ABaseInteractiveActor::AddAnimatedComponent(USceneComponent* NewComponent)
{
	if (AnimatedComponents.Contains(NewComponent)) return;

	AnimatedComponents.AddUnique(NewComponent);
}

void ABaseInteractiveActor::FillAnimatedComponents(TArray<USceneComponent*> Components)
{
	if (Components.Num() == 0) return;

	AnimatedComponents.Empty();

	for (const auto Component : Components)
	{
		if (!Component || AnimatedComponents.Contains(Component)) continue;

		AnimatedComponents.AddUnique(Component);
	}
}

void ABaseInteractiveActor::StartAnimation()
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

	SetState(EInteractiveActorState::Transition);
}

void ABaseInteractiveActor::ReverseAnimation()
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
}

void ABaseInteractiveActor::StopAnimation()
{
	AnimationTimeline->Stop();
}

void ABaseInteractiveActor::FinishAnimation()
{
	SetState(StateTarget);
}

void ABaseInteractiveActor::CalculatePlayRate()
{
	if (!AnimationTimeline) return;

	AnimationTimeline->SetPlayRate(1.f / AnimationDuration);
}

void ABaseInteractiveActor::AnimateTransform(const float AnimationProgress)
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

		AnimatedComponents[i]->SetRelativeTransform(NewTransform);
	}
}

bool ABaseInteractiveActor::CanBeReversed() const
{
	return IsStateCurrent(EInteractiveActorState::Transition) && bIsReversible;
}

void ABaseInteractiveActor::SetState(const EInteractiveActorState NewState)
{
	if (IsStateCurrent(NewState)) return;

	StatePrevious = StateCurrent;
	StateCurrent = NewState;
	OnActorChangedState.Broadcast(NewState);
	OnStateChanged(NewState);
}

void ABaseInteractiveActor::SetTargetState()
{
	StateTarget = StateCurrent == EInteractiveActorState::Closed
		              ? EInteractiveActorState::Opened
		              : EInteractiveActorState::Closed;
}

bool ABaseInteractiveActor::CanStartAnimation() const
{
	return !IsStateCurrent(EInteractiveActorState::Locked) || !IsStateCurrent(EInteractiveActorState::Disabled);
}

void ABaseInteractiveActor::Open()
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

void ABaseInteractiveActor::Close()
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

void ABaseInteractiveActor::Lock()
{
	if (!IsStateCurrent(EInteractiveActorState::Closed)) return;

	SetState(EInteractiveActorState::Locked);
}

void ABaseInteractiveActor::Unlock()
{
	if (!IsStateCurrent(EInteractiveActorState::Locked)) return;

	SetState(EInteractiveActorState::Closed);
}

void ABaseInteractiveActor::Enable()
{
	if (IsStateCurrent(EInteractiveActorState::Disabled)) return;

	SetState(StatePrevious);
}

void ABaseInteractiveActor::Disable()
{
	if (!IsStateCurrent(EInteractiveActorState::Transition)) return;

	SetState(EInteractiveActorState::Disabled);
}
