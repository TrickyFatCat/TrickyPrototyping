// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

class UCurveFloat;
class UTimelineComponent;

/**
 * This is a base class for interactive objects such as doors, buttons, etc.
 * It contains the basic logic for controlling animation for this objects.
 */

UCLASS()
class TRICKYPROTOTYPING_API AInteractiveActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Animation
public:
	UFUNCTION(BlueprintGetter, Category="Animation")
	float GetAnimationDuration() const { return AnimationDuration; }

	UFUNCTION(BlueprintSetter, Category="Animation")
	void SetAnimationDuration(const float Value);

	UFUNCTION(BlueprintGetter, Category="Animation")
	bool GetIsRevertible() const { return bIsRevertible; }

	UFUNCTION(BlueprintSetter, Category="Animation")
	void SetIsRevertible(const bool Value) { bIsRevertible = Value; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UTimelineComponent* AnimationTimeline = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	UCurveFloat* AnimationCurve = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	TSet<USceneComponent*> AnimatedComponents;
	
	UPROPERTY()
	TArray<FTransform> InitialTransforms;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	TArray<FTransform> TargetTransforms;
	
	virtual void StartAnimation();
	virtual void RevertAnimation();
	virtual void StopAnimation();
	virtual void FinishAnimation();

private:
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetAnimationDuration,
		BlueprintSetter=SetAnimationDuration,
		Category="Animation",
		meta=(AllowPrivateAccess="true"))
	float AnimationDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetIsRevertible,
		BlueprintSetter=SetIsRevertible,
		Category="Animation",
		meta=(AllowPrivateAccess="true"))
	bool bIsRevertible = false;
	
	UFUNCTION(BlueprintCallable, Category="Animation")
	void CalculatePlayRate();
	
	UFUNCTION()
	void AnimateTransform(const float AnimationProgress);
};
