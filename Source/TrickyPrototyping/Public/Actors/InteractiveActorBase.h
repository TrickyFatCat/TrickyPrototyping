// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActorBase.generated.h"

class UCurveFloat;
class UTimelineComponent;

/**
 * This is a base class for interactive objects such as doors, buttons, etc.
 * It contains the basic logic for controlling animation for this objects.
 */

UENUM(BlueprintType)
enum class EInteractiveActorState : uint8
{
	Opened,
	Closed,
	Locked,
	Disabled,
	Transition UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FInteractiveActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	bool bAnimateLocation = true;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bAnimateLocation"))
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	bool bAnimateRotation = true;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bAnimateRotation"))
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	bool bAnimateScale = false;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bAnimateScale"))
	FVector TargetScale = FVector(1.f, 1.f, 1.f);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractiveActorChangedStateSignature, EInteractiveActorState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransitionStartedSignature, EInteractiveActorState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransitionReversedSignature, EInteractiveActorState, TargetState);

UCLASS()
class TRICKYPROTOTYPING_API AInteractiveActorBase : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveActorBase();

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
	bool GetIsReversible() const { return bIsReversible; }

	UFUNCTION(BlueprintSetter, Category="Animation")
	void SetIsReversible(const bool Value) { bIsReversible = Value; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UTimelineComponent* AnimationTimeline = nullptr;

	UFUNCTION(BlueprintCallable, Category="Animation")
	void AddAnimatedComponent(USceneComponent* NewComponent);

	UFUNCTION(BlueprintCallable, Category="Animation")
	void FillAnimatedComponents(TArray<USceneComponent*> Components);

	virtual void StartAnimation();
	virtual void ReverseAnimation();
	virtual void StopAnimation();

	UFUNCTION()
	virtual void FinishAnimation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	UCurveFloat* AnimationCurve = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	TArray<USceneComponent*> AnimatedComponents;

	UPROPERTY()
	TArray<FTransform> InitialTransforms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	TArray<FInteractiveActorData> TargetTransforms;

	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetAnimationDuration,
		BlueprintSetter=SetAnimationDuration,
		Category="Animation",
		meta=(AllowPrivateAccess="true"))
	float AnimationDuration = 1.f;

	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetIsReversible,
		BlueprintSetter=SetIsReversible,
		Category="Animation",
		meta=(AllowPrivateAccess="true"))
	bool bIsReversible = false;

	UFUNCTION(BlueprintCallable, Category="Animation")
	void CalculatePlayRate();

	UFUNCTION()
	void AnimateTransform(const float AnimationProgress);

	bool CanBeReversed() const;

	// States
public:
	UFUNCTION(BlueprintGetter, Category="States")
	EInteractiveActorState GetStateCurrent() const { return StateCurrent; }

	UFUNCTION(BlueprintGetter, Category="States")
	EInteractiveActorState GetStateTarget() const { return StateTarget; }

	UFUNCTION(BlueprintGetter, Category="States")
	EInteractiveActorState GetStatePrevious() const { return StatePrevious; }

	UPROPERTY(BlueprintAssignable, Category="States")
	FOnInteractiveActorChangedStateSignature OnActorChangedState;

	UPROPERTY(BlueprintAssignable, Category="States")
	FOnTransitionStartedSignature OnActorTransitionStarted;

	UPROPERTY(BlueprintAssignable, Category="States")
	FOnTransitionReversedSignature OnActorTransitionReversed;

	UFUNCTION(BlueprintCallable, Category="States")
	bool IsStateCurrent(const EInteractiveActorState DoorState) const { return StateCurrent == DoorState; }

	UFUNCTION(BlueprintCallable, Category="States")
	bool IsStatePrevious(const EInteractiveActorState DoorState) const { return StatePrevious == DoorState; }

	UFUNCTION(BlueprintCallable, Category="States")
	bool IsStateTarget(const EInteractiveActorState DoorState) const { return StateTarget == DoorState; }

	UFUNCTION(BlueprintImplementableEvent, Category="States")
	void OnStateChanged(const EInteractiveActorState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category="States")
	void OnTransitionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="States")
	void OnTransitionReversed();
protected:
	UFUNCTION(BlueprintCallable, Category="States")
	void SetState(const EInteractiveActorState NewState);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateInitial = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStateCurrent, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateCurrent = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStatePrevious, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StatePrevious = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStateTarget, Category="States", meta=(AllowPrivateAccess="true"))
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
