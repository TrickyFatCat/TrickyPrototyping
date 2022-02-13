// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingPlatformBase.h"
#include "Components/TimelineComponent.h"


AFloatingPlatformBase::AFloatingPlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformRoot = CreateDefaultSubobject<USceneComponent>("PlatformRoot");
	SetRootComponent(PlatformRoot);

	MovementTimeline = CreateDefaultSubobject<UTimelineComponent>("MovementTimeline");
}

void AFloatingPlatformBase::BeginPlay()
{
	if (MovementTimeline && MovementAnimationCurve)
	{
		FOnTimelineFloat MovementProgress{};
		MovementProgress.BindUFunction(this, FName("MovePlatform"));
		MovementTimeline->AddInterpFloat(MovementAnimationCurve, MovementProgress);

		FOnTimelineEvent MovementFinished{};
		MovementFinished.BindUFunction(this, FName("ContinueMovement"));
		MovementTimeline->SetTimelineFinishedFunc(MovementFinished);

		CalculateTimelinePlayRate();
	}

	if (bAutoStart)
	{
		StartMovement();
	}
	
	Super::BeginPlay();
}

void AFloatingPlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloatingPlatformBase::StartMovement()
{
	if (State == EPlatformState::Moving) return;

	if (CurrentPointIndex == NextPointIndex && bStopAtPoints)
	{
		ContinueMovement();
	}
	
	CalculateTravelTime();
	CalculateTimelinePlayRate();
	MovementTimeline->PlayFromStart();
	SetState(EPlatformState::Moving);
}

void AFloatingPlatformBase::StopMovement()
{
	if (State == EPlatformState::Idle) return;

	MovementTimeline->Stop();
	SetState(EPlatformState::Idle);
}

void AFloatingPlatformBase::ResumeMovement()
{
	if (State != EPlatformState::Idle) return;

	MovementTimeline->Play();
	SetState(EPlatformState::Moving);
}

void AFloatingPlatformBase::MoveToPoint(const int32 PointIndex)
{
	if (!IndexIsValid(PointIndex)) return;

	NextPointIndex = PointIndex;
	CalculateTravelTime();
	StartMovement();
}

void AFloatingPlatformBase::SetSpeed(const float Value)
{
	if (Value < 0.f) return;

	Speed = Value;

	// TODO Recalculate movement timers;
}

bool AFloatingPlatformBase::IndexIsValid(const int32 Index)
{
	return Index > 0 && Index < PointsIndexes.Num();
}

void AFloatingPlatformBase::CalculateTravelTime()
{
}

void AFloatingPlatformBase::FillPointIndexes()
{
}

void AFloatingPlatformBase::MovePlatform(const float Progress)
{
}

void AFloatingPlatformBase::ContinueMovement()
{
	if (State == EPlatformState::Idle) return;

	OnPointReached.Broadcast(CurrentPointIndex);
	CalculateNextPointIndex();
	CalculateTravelTime();
	bStopAtPoints ? StartStopWaitTimer() : StartMovement();
}

void AFloatingPlatformBase::CalculateNextPointIndex()
{
	auto CalculateNextIndex = [&]() { NextPointIndex = bIsReversed ? CurrentPointIndex - 1 : CurrentPointIndex + 1; };

	CurrentPointIndex = NextPointIndex;
	CalculateNextIndex();
	const bool bIndexIsValid = IndexIsValid(NextPointIndex);

	switch (MovementMode)
	{
		case EPlatformMovementMode::Loop:
			if (!bIndexIsValid)
			{
				CurrentPointIndex = bIsReversed ? PointsIndexes.Num() - 1 : 0;
				CalculateNextIndex();
			}
			break;

		case EPlatformMovementMode::PingPong:
			if (!bIndexIsValid)
			{
				bIsReversed = !bIsReversed;
				CalculateNextIndex();
			}
			break;

		default:
			break;
	}
}

void AFloatingPlatformBase::StartStopWaitTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (StopWaitDuration > 0.f && !TimerManager.IsTimerActive(StopWaitTimerHandle))
	{
		OnWaitStarted.Broadcast(CurrentPointIndex);
		TimerManager.SetTimer(StopWaitTimerHandle, this, &AFloatingPlatformBase::FinishStopTimer, StopWaitDuration, false);
	}
}

void AFloatingPlatformBase::FinishStopTimer() const
{
	OnWaitFinished.Broadcast(CurrentPointIndex);
	MovementTimeline->PlayFromStart();
}

void AFloatingPlatformBase::SetState(const EPlatformState NewState)
{
	if (State == NewState) return;

	State = NewState;
	OnStateChanged.Broadcast(State);
}

void AFloatingPlatformBase::CalculateTimelinePlayRate()
{
	if (TravelTime <= 0.f)
	{
		TravelTime = 1.f;
		// TODO Print error;
	}
	
	MovementTimeline->SetPlayRate(1 / TravelTime);
}

