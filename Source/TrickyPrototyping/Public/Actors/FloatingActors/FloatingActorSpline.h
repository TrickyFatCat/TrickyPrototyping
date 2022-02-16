// Copyright (c) 202BeginPlay2 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/FloatingActorBase.h"
#include "FloatingActorSpline.generated.h"

class USplineComponent;

USTRUCT(BlueprintType)
struct FAxisInheritance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Inheritance")
	bool bInheritX = false;
	
	UPROPERTY(EditAnywhere, Category="Inheritance")
	bool bInheritY = false;
	
	UPROPERTY(EditAnywhere, Category="Inheritance")
	bool bInheritZ = false;
};

/**
 * The actor which moves along spline.
 * It's good for things which requires a complicated movement path.
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

	virtual void MoveActor(const float Progress) override;

	virtual void CalculateNextPointIndex() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingActor|Spline", meta=(AllowPrivateAccess="true"))
	AActor* SplineActor = nullptr;

	UPROPERTY()
	USplineComponent* TargetSplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingActor|Spline", meta=(AllowPrivateAccess="true"))
	FAxisInheritance InheritSplineRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingActor|Spline", meta=(AllowPrivateAccess="true"))
	FAxisInheritance InheritSplineScale;	

	float GetSplineDistance(const int32 PointIndex) const;

	float GetPositionAtSpline(const float Progress) const;

	void RotateAlongSpline(const float Progress);

	void ScaleAlongSpline(const float Progress);

protected:
};
