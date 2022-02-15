// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingActorBase.h"

#include "Components/TimelineComponent.h"
#include "Core/TrickyFunctionLibrary.h"


AFloatingActorBase::AFloatingActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorRoot = CreateDefaultSubobject<USceneComponent>("ActorRoot");
	SetRootComponent(ActorRoot);

	MovementTimeline = CreateDefaultSubobject<UTimelineComponent>("MovementTimeline");
}

void AFloatingActorBase::BeginPlay()
{
	if (MovementTimeline && MovementAnimationCurve)
	{
		FOnTimelineFloat MovementProgress{};
		MovementProgress.BindUFunction(this, FName("MovePlatform"));
		MovementTimeline->AddInterpFloat(MovementAnimationCurve, MovementProgress);

		FOnTimelineEvent MovementFinished{};
		MovementFinished.BindUFunction(this, FName("ContinueMovement"));
		MovementTimeline->SetTimelineFinishedFunc(MovementFinished);

		UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, MovementAnimationCurve, TravelTime);
	}

	if (WaitDuration <= 0.f)
	{
		bStopAtPoints = false;
	}

	if (bAutoStart && MovementMode != EFloatingActorMovementMode::Manual)
	{
		StartMovement();
	}

	Super::BeginPlay();
}

void AFloatingActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ConstructActor();
}

void AFloatingActorBase::ConstructActor()
{
	FillPointIndexes();

	if (PointsIndexes.Num() > 0)
	{
		if (!IndexIsValid(StartPointIndex))
		{
			StartPointIndex = 0;
			// TODO Print error.
		}

		CurrentPointIndex = StartPointIndex;
		NextPointIndex = StartPointIndex;
		MovePlatform(0.0f);

		if (bStopAtPoints && bStopAtCertainPoints && CustomStopsIndexes.Num() > 0)
		{
			RemoveInvalidCustomIndexes();

			// Sort custom indexes
			if (bSortCustomStops)
			{
				auto Iterator = [](const int32& LHS, const int32& RHS) { return LHS < RHS; };
				CustomStopsIndexes.Sort(Iterator);
			}
		}
	}
	else
	{
		// TODO Print error
	}
}

void AFloatingActorBase::StartMovement()
{
	if (State == EFloatingActorState::Moving || PointsIndexes.Num() == 0) return; // TODO Print error if Num == 0

	if (CurrentPointIndex == NextPointIndex)
	{
		CalculateNextPointIndex();
	}

	CalculateTravelTime();
	UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, MovementAnimationCurve, TravelTime);

	if (bStopAtPoints && bWaitAtStart)
	{
		StartStopWaitTimer();
		return;
	}

	MovementTimeline->PlayFromStart();
	SetState(EFloatingActorState::Moving);
}

void AFloatingActorBase::StopMovement()
{
	if (State == EFloatingActorState::Idle) return;

	MovementTimeline->Stop();
	SetState(EFloatingActorState::Idle);
}

void AFloatingActorBase::ResumeMovement()
{
	if (State != EFloatingActorState::Idle) return;

	MovementTimeline->Play();
	SetState(EFloatingActorState::Moving);
}

void AFloatingActorBase::MoveToPoint(const int32 PointIndex)
{
	if (!IndexIsValid(PointIndex) || MovementMode != EFloatingActorMovementMode::Manual) return;

	NextPointIndex = PointIndex;
	CalculateTravelTime();
	UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, MovementAnimationCurve, TravelTime);
	SetState(EFloatingActorState::Moving);
	MovementTimeline->PlayFromStart();
}

void AFloatingActorBase::SetSpeed(const float Value)
{
	if (Value < 0.f) return;

	Speed = Value;
	CalculateTravelTime();
}

void AFloatingActorBase::SetTravelTime(const float Value)
{
	if (bUseTravelTime || Value <= 0.f) return;

	TravelTime = Value;
	UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, MovementAnimationCurve, TravelTime);
}

void AFloatingActorBase::SetWaitDuration(const float Value)
{
	if (Value < 0.f) return;

	WaitDuration = Value;

	if (WaitDuration == 0.f) bStopAtPoints = false;
}

bool AFloatingActorBase::IndexIsValid(const int32 Index) const
{
	return Index >= 0 && Index < PointsIndexes.Num();
}

void AFloatingActorBase::CalculateTravelTime()
{
}

void AFloatingActorBase::FillPointIndexes()
{
	PointsIndexes.Empty();
}

void AFloatingActorBase::RemoveInvalidCustomIndexes()
{
}

void AFloatingActorBase::MovePlatform(const float Progress)
{
}

void AFloatingActorBase::ContinueMovement()
{
	if (State == EFloatingActorState::Idle) return;

	OnPointReached.Broadcast(CurrentPointIndex);

	if (MovementMode == EFloatingActorMovementMode::Manual)
	{
		CurrentPointIndex = NextPointIndex;
		SetState(EFloatingActorState::Idle);
		return;
	}

	CalculateNextPointIndex();
	CalculateTravelTime();

	if (bStopAtPoints)
	{
		StartStopWaitTimer();
	}
	else
	{
		MovementTimeline->PlayFromStart();
	}
}

void AFloatingActorBase::CalculateNextPointIndex()
{
	auto CalculateNextIndex = [&]() { NextPointIndex = bIsReversed ? CurrentPointIndex - 1 : CurrentPointIndex + 1; };

	CurrentPointIndex = NextPointIndex;
	CalculateNextIndex();
	const bool bIndexIsValid = IndexIsValid(NextPointIndex);

	switch (MovementMode)
	{
		case EFloatingActorMovementMode::Loop:
			if (!bIndexIsValid)
			{
				CurrentPointIndex = bIsReversed ? PointsIndexes.Num() - 1 : 0;
				CalculateNextIndex();
			}
			break;

		case EFloatingActorMovementMode::PingPong:
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

void AFloatingActorBase::StartStopWaitTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (!TimerManager.IsTimerActive(WaitTimerHandle))
	{
		OnWaitStarted.Broadcast(CurrentPointIndex);
		SetState(EFloatingActorState::Waiting);
		TimerManager.SetTimer(WaitTimerHandle,
		                      this,
		                      &AFloatingActorBase::FinishStopTimer,
		                      WaitDuration,
		                      false);
	}
}

void AFloatingActorBase::FinishStopTimer()
{
	OnWaitFinished.Broadcast(CurrentPointIndex);
	SetState(EFloatingActorState::Moving);
	MovementTimeline->PlayFromStart();
}

void AFloatingActorBase::SetState(const EFloatingActorState NewState)
{
	if (State == NewState) return;

	State = NewState;
	OnStateChanged.Broadcast(State);
}
