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

void AInteractiveActor::StartAnimation()
{
	if (!AnimationCurve) return;
	
	AnimationTimeline->PlayFromStart();
}

void AInteractiveActor::RevertAnimation()
{
	if (!AnimationCurve) return;
	
	AnimationTimeline->Reverse();
}

void AInteractiveActor::StopAnimation()
{
	AnimationTimeline->Stop();
}

void AInteractiveActor::FinishAnimation()
{
	
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

		FQuat NewRotation = FRotator(NewTransform.GetRotation().Rotator() + TargetTransform.GetRotation().Rotator() * AnimationProgress).Quaternion();
		NewTransform.SetRotation(NewRotation);

		NewTransform.SetScale3D(NewTransform.GetScale3D() + TargetTransform.GetScale3D() * AnimationProgress);

		AnimatedComponents.Array()[i]->SetRelativeTransform(NewTransform);
	}
}
