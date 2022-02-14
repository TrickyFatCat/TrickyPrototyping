// Copyright (c) 202BeginPlay2 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/FloatingActorBase.h"
#include "FloatingActorSpline.generated.h"

class USplineComponent;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TRICKYPROTOTYPING_API AFloatingActorSpline : public AFloatingActorBase
{
	GENERATED_BODY()

protected:
	virtual void InitPlatform() override;
	
public:
	UFUNCTION(BlueprintGetter, Category="FloatingPlatform|Spline")
	AActor* GetSplineActor() const { return SplineActor; }

	UFUNCTION(BlueprintSetter, Category="FloatingPlatform|Spline")
	void SetSplineActor(AActor* Value);

protected:
	virtual void CalculateTravelTime() override;

	virtual void FillPointIndexes() override;

	virtual void RemoveInvalidCustomIndexes() override;

	virtual void MovePlatform(const float Progress) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingPlatform", meta=(AllowPrivateAccess="true"))
	AActor* SplineActor = nullptr;

	UPROPERTY()
	USplineComponent* SplineComponent = nullptr;

	float GetSplineDistance(const int32 PointIndex) const;

protected:
};
