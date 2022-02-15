// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingActors/FloatingActorSpline.h"

#include "Components/SplineComponent.h"
#include "Core/TrickyUtils.h"

void AFloatingActorSpline::ConstructActor()
{
	if (SplineActor)
	{
		SplineComponent = FTrickyUtils::GetComponent<USplineComponent>(SplineActor);
	}
	Super::ConstructActor();
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

	if (!SplineComponent || bUseTravelTime) return;

	const float StartDistance = GetSplineDistance(CurrentPointIndex);
	const float FinishDistance = GetSplineDistance(NextPointIndex);
	const float DistanceBetweenPoints = FMath::Abs(FinishDistance - StartDistance);
	TravelTime = DistanceBetweenPoints / Speed;
}

void AFloatingActorSpline::FillPointIndexes()
{
	if (!SplineComponent) return; // TODO Print error.

	Super::FillPointIndexes();

	auto GetPointsFromSpline = [&]()
	{
		for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
		{
			PointsIndexes.Add(i);
		}

		if (SplineComponent->IsClosedLoop())
		{
			PointsIndexes.Add(SplineComponent->GetNumberOfSplinePoints());
		}
	};

	if (MovementMode == EFloatingActorMovementMode::Manual)
	{
		GetPointsFromSpline();
	}
	else
	{
		if (bStopAtPoints)
		{
			if (bStopAtCertainPoints && CustomStopsIndexes.Num() == 0)
			{
				return; // TODO Print error;
			}

			if (bStopAtCertainPoints && CustomStopsIndexes.Num() > 1)
			{
				PointsIndexes = CustomStopsIndexes;
				return;
			}

			GetPointsFromSpline();
		}
		else
		{
			PointsIndexes.Add(0);
			const int PointsNumber = SplineComponent->GetNumberOfSplinePoints();
			PointsIndexes.Add(SplineComponent->IsClosedLoop() ? PointsNumber : PointsNumber - 1);
		}
	}
}

void AFloatingActorSpline::RemoveInvalidCustomIndexes()
{
	Super::RemoveInvalidCustomIndexes();

	for (int32 i = 0; i < CustomStopsIndexes.Num(); i++)
	{
		const int Index = CustomStopsIndexes[i];

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

	const FVector NewLocation{
		SplineComponent->GetLocationAtDistanceAlongSpline(GetPositionAtSpline(Progress),
		                                                  ESplineCoordinateSpace::World)
	};
	SetActorLocation(NewLocation);
	RotateAlongSpline(Progress);
	ScaleAlongSpline(Progress);
}

float AFloatingActorSpline::GetSplineDistance(const int32 PointIndex) const
{
	if (!SplineComponent) return -1.f;

	return SplineComponent->GetDistanceAlongSplineAtSplinePoint(PointsIndexes[PointIndex]);
}

float AFloatingActorSpline::GetPositionAtSpline(const float Progress) const
{
	if (!SplineComponent) return -1;

	const float Start = GetSplineDistance(CurrentPointIndex);
	const float Finish = GetSplineDistance(NextPointIndex);

	return FMath::Lerp(Start, Finish, Progress);
}

void AFloatingActorSpline::RotateAlongSpline(const float Progress)
{
	if (!SplineComponent) return;

	if (InheritSplineRotation.bInheritX || InheritSplineRotation.bInheritY || InheritSplineRotation.bInheritZ)
	{
		const FRotator CurrentRotation{GetActorRotation()};
		const FRotator RotationAlongSpline{
			SplineComponent->GetRotationAtDistanceAlongSpline(GetPositionAtSpline(Progress),
			                                                  ESplineCoordinateSpace::World)
		};
		const float NewRoll = InheritSplineRotation.bInheritX ? RotationAlongSpline.Roll : CurrentRotation.Roll;
		const float NewPitch = InheritSplineRotation.bInheritY ? RotationAlongSpline.Pitch : CurrentRotation.Pitch;
		const float NewYaw= InheritSplineRotation.bInheritZ ? RotationAlongSpline.Yaw : CurrentRotation.Yaw;
		SetActorRotation(FRotator{NewPitch, NewYaw, NewRoll});
	}
}

void AFloatingActorSpline::ScaleAlongSpline(const float Progress)
{
	if (!SplineComponent) return;

	if (InheritSplineScale.bInheritX || InheritSplineScale.bInheritY || InheritSplineScale.bInheritZ)
	{
		const FVector CurrentScale{GetActorScale3D()};
		const FVector ScaleAlongSpline{SplineComponent->GetScaleAtDistanceAlongSpline(GetPositionAtSpline(Progress))};
		const float NewScaleX = InheritSplineScale.bInheritX ? ScaleAlongSpline.X : CurrentScale.X;
		const float NewScaleY = InheritSplineScale.bInheritY ? ScaleAlongSpline.Y : CurrentScale.Y;
		const float NewScaleZ = InheritSplineScale.bInheritZ ? ScaleAlongSpline.Z : CurrentScale.Z;
		SetActorScale3D(FVector{NewScaleX, NewScaleY, NewScaleZ});
	}
}
