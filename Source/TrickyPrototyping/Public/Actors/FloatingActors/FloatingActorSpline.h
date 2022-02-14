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
	virtual void ConstructActor() override;
	
public:
	UFUNCTION(BlueprintGetter, Category="FloatingActor|Spline")
	AActor* GetSplineActor() const { return SplineActor; }

	UFUNCTION(BlueprintSetter, Category="FloatingActor|Spline")
	void SetSplineActor(AActor* Value);

protected:
	virtual void CalculateTravelTime() override;

	virtual void FillPointIndexes() override;

	virtual void RemoveInvalidCustomIndexes() override;

	virtual void MovePlatform(const float Progress) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingActor|Spline", meta=(AllowPrivateAccess="true"))
	AActor* SplineActor = nullptr;

	UPROPERTY()
	USplineComponent* SplineComponent = nullptr;

	float GetSplineDistance(const int32 PointIndex) const;

protected:
};
