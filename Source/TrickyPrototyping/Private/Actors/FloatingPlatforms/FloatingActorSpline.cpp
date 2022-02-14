// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingPlatforms/FloatingActorSpline.h"

#include "Components/SplineComponent.h"
#include "Core/TrickyUtils.h"

void AFloatingActorSpline::InitPlatform()
{
	if (SplineActor)
	{
		SplineComponent = FTrickyUtils::GetComponent<USplineComponent>(SplineActor);
	}
	Super::InitPlatform();
}

void AFloatingActorSpline::SetSplineActor(AActor* Value)
{
	if (!Value) return;

	SplineActor = Value;
	SplineComponent = FTrickyUtils::GetComponent<USplineComponent>(Value);
}

void AFloatingActorSpline::CalculateTravelTime()
{
	Super::CalculateTravelTime();

	if (!SplineComponent) return;

	const float StartDistance = GetSplineDistance(CurrentPointIndex);
	const float FinishDistance = GetSplineDistance(NextPointIndex);
	const float DistanceBetweenPoints = FMath::Abs(FinishDistance - StartDistance);
	TravelTime = DistanceBetweenPoints / Speed;
}

void AFloatingActorSpline::FillPointIndexes()
{
	if (!SplineComponent) return; // TODO Print error.

	Super::FillPointIndexes();

	for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
	{
		PointsIndexes.Add(i);
	}

	if (SplineComponent->IsClosedLoop())
	{
		PointsIndexes.Add(SplineComponent->GetNumberOfSplinePoints());
	}
}

void AFloatingActorSpline::RemoveInvalidCustomIndexes()
{
	Super::RemoveInvalidCustomIndexes();

	for (const int32& Index : CustomStopsIndexes)
	{
		if (Index < 0)
		{
			CustomStopsIndexes.Remove(Index);
			continue;
		}

		if (!SplineComponent) continue; // TODO Print error;

		const int32 NumberOfPoints = SplineComponent->GetNumberOfSplinePoints();
		const bool bIsValid = SplineComponent->IsClosedLoop() ? Index <= NumberOfPoints : Index < NumberOfPoints;

		if (!bIsValid)
		{
			CustomStopsIndexes.Remove(Index);
		}
	}
}

void AFloatingActorSpline::MovePlatform(const float Progress)
{
	Super::MovePlatform(Progress);

	if (!SplineComponent) return; // TODO Print error.

	const float Start = GetSplineDistance(CurrentPointIndex);
	const float Finish = GetSplineDistance(NextPointIndex);
	const float SplinePosition = FMath::Lerp(Start, Finish, Progress);
	const FVector NewLocation{
		SplineComponent->GetLocationAtDistanceAlongSpline(SplinePosition,
		                                                  ESplineCoordinateSpace::World)
	};
	SetActorLocation(NewLocation);
}

float AFloatingActorSpline::GetSplineDistance(const int32 PointIndex) const
{
	if (!SplineComponent) return -1.f;

	return SplineComponent->GetDistanceAlongSplineAtSplinePoint(PointsIndexes[PointIndex]);
}
