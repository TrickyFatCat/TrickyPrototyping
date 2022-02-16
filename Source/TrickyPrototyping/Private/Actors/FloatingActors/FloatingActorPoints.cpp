// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingActors/FloatingActorPoints.h"

void AFloatingActorPoints::ConstructActor()
{
	Super::ConstructActor();

	if (TargetActors.Num() > 0)
	{
		if (!TargetActors[StartPointIndex])
		{
			LogError(FString::Printf(TEXT("The start index %d contains a nullptr actor"), StartPointIndex));
		}
	}
}

void AFloatingActorPoints::CalculateTravelTime()
{
	Super::CalculateTravelTime();

	const AActor* StartActor = GetTargetActor(CurrentPointIndex);
	const AActor* FinishActor = GetTargetActor(NextPointIndex);

	if (bUseTravelTime || !StartActor || !FinishActor) return;

	const float Distance = StartActor->GetDistanceTo(FinishActor);
	TravelTime = Distance / Speed;
}

void AFloatingActorPoints::FillPointIndexes()
{
	Super::FillPointIndexes();

	if (TargetActors.Num() < 2)
	{
		LogError("Not enough target points. There must be at least 2 to move between.");
		return;
	}

	auto FillIndexes = [&]()
	{
		for (int32 i = 0; i < TargetActors.Num(); i++)
		{
			if (!TargetActors[i])
			{
				LogError(FString::Printf(TEXT("Actor with index %d is nullptr. It was skipped."), i));
				continue;
			}
			PointsIndexes.Add(i);
		}
	};

	switch (MovementMode)
	{
		case EFloatingActorMovementMode::Manual:
			FillIndexes();
			break;

		default:
			if (bStopAtPoints && bStopAtCertainPoints)
			{
				if (CustomStopsIndexes.Num() < 2) return;

				PointsIndexes = CustomStopsIndexes;
				SortPointsIndexes();

				return;
			}

			FillIndexes();
			break;
	}
}

void AFloatingActorPoints::RemoveInvalidCustomIndexes()
{
	Super::RemoveInvalidCustomIndexes();

	for (int32 i = 0; i < CustomStopsIndexes.Num(); i++)
	{
		const int32 Index = CustomStopsIndexes[i];

		if (Index < 0 || Index >= TargetActors.Num())
		{
			CustomStopsIndexes.Remove(Index);
			continue;
		}

		if (!TargetActors[Index])
		{
			LogError(FString::Printf(TEXT("The cosen index %d contains nullptr actor."), Index));
			CustomStopsIndexes.Remove(Index);
		}
	}
}

void AFloatingActorPoints::MoveActor(const float Progress)
{
	Super::MoveActor(Progress);

	const AActor* StartActor = GetTargetActor(CurrentPointIndex);
	const AActor* FinishActor = GetTargetActor(NextPointIndex);

	if (TargetActors.Num() < 2 || !StartActor || !FinishActor) return;

	const FVector StartPosition = StartActor->GetActorLocation();
	const FVector FinishPosition = FinishActor->GetActorLocation();
	SetActorLocation(FMath::Lerp(StartPosition, FinishPosition, Progress));
}

void AFloatingActorPoints::CalculateNextPointIndex()
{
	auto CalculateNextIndex = [&]() { NextPointIndex = bIsReversed ? CurrentPointIndex - 1 : CurrentPointIndex + 1; };

	CurrentPointIndex = NextPointIndex;

	switch (MovementMode)
	{
		case EFloatingActorMovementMode::Loop:
			CalculateNextIndex();

			if (NextPointIndex >= PointsIndexes.Num())
			{
				NextPointIndex = 0;
			}
			else if (NextPointIndex < 0)
			{
				NextPointIndex = PointsIndexes.Num() - 1;
			}
			break;

		case EFloatingActorMovementMode::PingPong:
			CalculateNextIndex();
			
			if (!IndexIsValid(NextPointIndex))
			{
				bIsReversed = !bIsReversed;
				CalculateNextIndex();
			}
			break;

		default:
			break;
	}
}

AActor* AFloatingActorPoints::GetTargetActor(const int32 PointIndex) const
{
	if (PointIndex < 0 || PointIndex >= TargetActors.Num() || TargetActors.Num() == 0) return nullptr;

	return TargetActors[PointsIndexes[PointIndex]];
}
