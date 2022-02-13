// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingPlatforms/FloatingPlatformSpline.h"

#include "Components/SplineComponent.h"
#include "Core/TrickyUtils.h"

void AFloatingPlatformSpline::InitPlatform()
{
	if (SplineActor)
	{
		SplineComponent = FTrickyUtils::GetComponent<USplineComponent>(SplineActor);
	}
	Super::InitPlatform();
}

void AFloatingPlatformSpline::SetSplineActor(AActor* Value)
{
	if (!Value) return;

	SplineActor = Value;
	SplineComponent = FTrickyUtils::GetComponent<USplineComponent>(Value);
}

void AFloatingPlatformSpline::CalculateTravelTime()
{
	Super::CalculateTravelTime();

	if (!SplineComponent) return;

	const float StartDistance = GetSplineDistance(CurrentPointIndex);
	const float FinishDistance = GetSplineDistance(NextPointIndex);
	const float DistanceBetweenPoints = FMath::Abs(FinishDistance - StartDistance);
	TravelTime = DistanceBetweenPoints / Speed;
}

void AFloatingPlatformSpline::FillPointIndexes()
{
	if (!SplineComponent) return; // TODO Print error.

	Super::FillPointIndexes();

	if (bStopAtCertainPoints)
	{
		PointsIndexes = CustomPointsIndexes.Array();
	}
	else
	{
		for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
		{
			PointsIndexes.Add(i);
		}
	}

	if (SplineComponent->IsClosedLoop())
	{
		PointsIndexes.Add(SplineComponent->GetNumberOfSplinePoints());
	}
}

void AFloatingPlatformSpline::MovePlatform(const float Progress)
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

float AFloatingPlatformSpline::GetSplineDistance(const int32 PointIndex) const
{
	if (!SplineComponent || !IndexIsValid(PointIndex)) return -1.f;

	return SplineComponent->GetDistanceAlongSplineAtSplinePoint(PointsIndexes[PointIndex]);
}
