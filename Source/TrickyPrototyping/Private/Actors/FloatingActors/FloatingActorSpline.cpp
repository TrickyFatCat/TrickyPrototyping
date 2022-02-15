// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingActors/FloatingActorSpline.h"

#include "Components/SplineComponent.h"
#include "Core/TrickyUtils.h"

void AFloatingActorSpline::ConstructActor()
{
	if (SplineActor)
	{
		TargetSplineComponent = FTrickyUtils::GetComponent<USplineComponent>(SplineActor);
	}
	Super::ConstructActor();
}

void AFloatingActorSpline::SetSplineActor(AActor* Value)
{
	if (!Value) return;

	SplineActor = Value;
	TargetSplineComponent = FTrickyUtils::GetComponent<USplineComponent>(Value);
}

void AFloatingActorSpline::CalculateTravelTime()
{
	Super::CalculateTravelTime();

	if (!TargetSplineComponent || bUseTravelTime) return;

	const float StartDistance = GetSplineDistance(CurrentPointIndex);
	const float FinishDistance = GetSplineDistance(NextPointIndex);
	const float DistanceBetweenPoints = FMath::Abs(FinishDistance - StartDistance);
	TravelTime = DistanceBetweenPoints / Speed;
}

void AFloatingActorSpline::FillPointIndexes()
{
	if (!TargetSplineComponent) return; // TODO Print error.

	PointsIndexes.Empty();

	auto GetPointsFromSpline = [&]()
	{
		for (int32 i = 0; i < TargetSplineComponent->GetNumberOfSplinePoints(); i++)
		{
			PointsIndexes.Add(i);
		}

		if (TargetSplineComponent->IsClosedLoop())
		{
			PointsIndexes.Add(TargetSplineComponent->GetNumberOfSplinePoints());
		}
	};

	switch (MovementMode)
	{
		case EFloatingActorMovementMode::Manual:
			GetPointsFromSpline();
			break;

		default:
			if (bStopAtPoints)
			{
				if (bStopAtCertainPoints)
				{
					if (CustomStopsIndexes.Num() < 2)
					{
						return; // TODO Print error;
					}

					PointsIndexes = CustomStopsIndexes;
					SortPointsIndexes();
					
					return;
				}

				GetPointsFromSpline();
			}
			else
			{
				PointsIndexes.Add(0);
				const int PointsNumber = TargetSplineComponent->GetNumberOfSplinePoints();
				PointsIndexes.Add(TargetSplineComponent->IsClosedLoop() ? PointsNumber : PointsNumber - 1);
			}
			break;
	}
}

void AFloatingActorSpline::RemoveInvalidCustomIndexes()
{
	Super::RemoveInvalidCustomIndexes();

	for (int32 i = 0; i < CustomStopsIndexes.Num(); i++)
	{
		const int32 Index = CustomStopsIndexes[i];

		if (Index < 0)
		{
			CustomStopsIndexes.Remove(Index);
			continue;
		}

		if (!TargetSplineComponent) continue; // TODO Print error;

		const int32 NumberOfPoints = TargetSplineComponent->GetNumberOfSplinePoints();
		const bool bIsValid = TargetSplineComponent->IsClosedLoop() ? Index <= NumberOfPoints : Index < NumberOfPoints;

		if (!bIsValid)
		{
			CustomStopsIndexes.Remove(Index);
		}
	}
}

void AFloatingActorSpline::MovePlatform(const float Progress)
{
	Super::MovePlatform(Progress);

	if (!TargetSplineComponent) return; // TODO Print error.

	const FVector NewLocation{
		TargetSplineComponent->GetLocationAtDistanceAlongSpline(GetPositionAtSpline(Progress),
		                                                        ESplineCoordinateSpace::World)
	};
	SetActorLocation(NewLocation);
	RotateAlongSpline(Progress);
	ScaleAlongSpline(Progress);
}

float AFloatingActorSpline::GetSplineDistance(const int32 PointIndex) const
{
	if (!TargetSplineComponent) return -1.f;

	return TargetSplineComponent->GetDistanceAlongSplineAtSplinePoint(PointsIndexes[PointIndex]);
}

float AFloatingActorSpline::GetPositionAtSpline(const float Progress) const
{
	if (!TargetSplineComponent) return -1;

	const float Start = GetSplineDistance(CurrentPointIndex);
	const float Finish = GetSplineDistance(NextPointIndex);

	return FMath::Lerp(Start, Finish, Progress);
}

void AFloatingActorSpline::RotateAlongSpline(const float Progress)
{
	if (!TargetSplineComponent) return;

	if (InheritSplineRotation.bInheritX || InheritSplineRotation.bInheritY || InheritSplineRotation.bInheritZ)
	{
		const FRotator CurrentRotation{GetActorRotation()};
		const FRotator RotationAlongSpline{
			TargetSplineComponent->GetRotationAtDistanceAlongSpline(GetPositionAtSpline(Progress),
			                                                        ESplineCoordinateSpace::World)
		};
		const float NewRoll = InheritSplineRotation.bInheritX ? RotationAlongSpline.Roll : CurrentRotation.Roll;
		const float NewPitch = InheritSplineRotation.bInheritY ? RotationAlongSpline.Pitch : CurrentRotation.Pitch;
		const float NewYaw = InheritSplineRotation.bInheritZ ? RotationAlongSpline.Yaw : CurrentRotation.Yaw;
		SetActorRotation(FRotator{NewPitch, NewYaw, NewRoll});
	}
}

void AFloatingActorSpline::ScaleAlongSpline(const float Progress)
{
	if (!TargetSplineComponent) return;

	if (InheritSplineScale.bInheritX || InheritSplineScale.bInheritY || InheritSplineScale.bInheritZ)
	{
		const FVector CurrentScale{GetActorScale3D()};
		const FVector ScaleAlongSpline{
			TargetSplineComponent->GetScaleAtDistanceAlongSpline(GetPositionAtSpline(Progress))
		};
		const float NewScaleX = InheritSplineScale.bInheritX ? ScaleAlongSpline.X : CurrentScale.X;
		const float NewScaleY = InheritSplineScale.bInheritY ? ScaleAlongSpline.Y : CurrentScale.Y;
		const float NewScaleZ = InheritSplineScale.bInheritZ ? ScaleAlongSpline.Z : CurrentScale.Z;
		SetActorScale3D(FVector{NewScaleX, NewScaleY, NewScaleZ});
	}
}
