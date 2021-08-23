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

UENUM()
enum class EInteractiveActorState : uint8
{
	Opened,
	Closed,
	Locked,
	Disabled,
	Transition UMETA(Hidden)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractiveActorChangedStateSignature, EInteractiveActorState, NewState);

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

	UFUNCTION(BlueprintCallable, Category="Animation")
	void SetAnimationProgress(const float Progress);

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
	virtual void ReverseAnimation();
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

	bool CanBeReversed() const;

	// States
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="States")
	EInteractiveActorState StateInitial = EInteractiveActorState::Closed;

	UFUNCTION(BlueprintCallable, Category="States")
	EInteractiveActorState GetStateCurrent() const { return StateCurrent; }

	UPROPERTY(BlueprintAssignable, Category="States")
	FOnInteractiveActorChangedStateSignature OnActorChangedState;

	UFUNCTION(BlueprintCallable, Category="States")
	bool IsStateCurrent(const EInteractiveActorState DoorState) const { return StateCurrent == DoorState; }

	UFUNCTION(BlueprintImplementableEvent, Category="States")
	void OnStateChanged(const EInteractiveActorState NewState);

protected:
	UFUNCTION(BlueprintCallable, Category="States")
	void SetState(const EInteractiveActorState NewState);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateCurrent = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StatePrevious = EInteractiveActorState::Transition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateTarget = EInteractiveActorState::Opened;

	void SetTargetState();
	bool CanStartAnimation() const;

	// Actions
public:
	UFUNCTION(BlueprintCallable, Category="Actions")
	virtual void Open();
	UFUNCTION(BlueprintCallable, Category="Actions")
	virtual void Close();
	UFUNCTION(BlueprintCallable, Category="Actions")
	virtual void Lock();
	UFUNCTION(BlueprintCallable, Category="Actions")
	virtual void Unlock();
	UFUNCTION(BlueprintCallable, Category="Actions")
	virtual void Enable();
	UFUNCTION(BlueprintCallable, Category="Actions")
	virtual void Disable();
	
protected:
private:
};
