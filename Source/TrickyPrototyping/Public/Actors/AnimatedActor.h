// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimatedActor.generated.h"

class UCurveFloat;
class UTimelineComponent;

/**
 * This is a base class for interactive objects such as doors, buttons, etc.
 * It contains the basic logic for controlling animation for this objects.
 */

UENUM(BlueprintType)
enum class EAnimatedActorState : uint8
{
	Opened,
	Closed,
	Locked,
	Disabled,
	Transition UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FAnimatedActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Animation")
	bool bAnimateLocation = true;

	UPROPERTY(EditAnywhere, Category="Animation", meta=(EditCondition="bAnimateLocation"))
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category="Animation")
	bool bAnimateRotation = false;

	UPROPERTY(EditAnywhere, Category="Animation", meta=(EditCondition="bAnimateRotation"))
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category="Animation")
	bool bAnimateScale = false;

	UPROPERTY(EditAnywhere, Category="Animation", meta=(EditCondition="bAnimateScale"))
	FVector ScaleOffset = FVector::ZeroVector;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimatedActorChangedStateSignature, EAnimatedActorState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransitionStartedSignature, EAnimatedActorState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransitionReversedSignature, EAnimatedActorState, TargetState);

UCLASS()
class TRICKYPROTOTYPING_API AAnimatedActor : public AActor
{
	GENERATED_BODY()

public:
	AAnimatedActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Animation
public:
	UFUNCTION(BlueprintGetter, Category="AnimatedActor|Animation")
	float GetAnimationDuration() const { return AnimationDuration; }

	UFUNCTION(BlueprintSetter, Category="AnimatedActor|Animation")
	void SetAnimationDuration(const float Value);

	UFUNCTION(BlueprintGetter, Category="AnimatedActor|Animation")
	bool GetIsReversible() const { return bIsReversible; }

	UFUNCTION(BlueprintSetter, Category="AnimatedActor|Animation")
	void SetIsReversible(const bool Value) { bIsReversible = Value; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UTimelineComponent* AnimationTimeline = nullptr;

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Animation")
	void AddAnimatedComponent(USceneComponent* NewComponent);

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Animation")
	void FillAnimatedComponents(TArray<USceneComponent*> Components);

	virtual void StartAnimation();

	virtual void ReverseAnimation();

	virtual void StopAnimation();

	UFUNCTION()
	virtual void FinishAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimatedActor|Animation", meta=(AllowPrivateAccess="true"))
	UCurveFloat* AnimationCurve = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimatedActor|Animation",
		meta=(AllowPrivateAccess="true"))
	TArray<USceneComponent*> AnimatedComponents;

	UPROPERTY()
	TArray<FTransform> InitialTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimatedActor|Animation", meta=(AllowPrivateAccess="true"))
	TArray<FAnimatedActorData> TransformOffsets;

	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetAnimationDuration,
		BlueprintSetter=SetAnimationDuration,
		Category="AnimatedActor|Animation",
		meta=(AllowPrivateAccess="true"))
	float AnimationDuration = 1.f;

	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetIsReversible,
		BlueprintSetter=SetIsReversible,
		Category="AnimatedActor|Animation",
		meta=(AllowPrivateAccess="true"))
	bool bIsReversible = false;

	UFUNCTION(BlueprintCallable, Category="Animation")
	void CalculatePlayRate();

	UFUNCTION()
	void AnimateTransform(const float AnimationProgress);

	bool CanBeReversed() const;

	// States
public:
	UFUNCTION(BlueprintGetter, Category="AnimatedActor|States")
	EAnimatedActorState GetStateCurrent() const { return StateCurrent; }

	UFUNCTION(BlueprintGetter, Category="AnimatedActor|States")
	EAnimatedActorState GetStateTarget() const { return StateTarget; }

	UFUNCTION(BlueprintGetter, Category="AnimatedActor|States")
	EAnimatedActorState GetStatePrevious() const { return StatePrevious; }

	UPROPERTY(BlueprintAssignable, Category="AnimatedActor|States")
	FOnAnimatedActorChangedStateSignature OnActorChangedState;

	UPROPERTY(BlueprintAssignable, Category="AnimatedActor|States")
	FOnTransitionStartedSignature OnActorTransitionStarted;

	UPROPERTY(BlueprintAssignable, Category="AnimatedActor|States")
	FOnTransitionReversedSignature OnActorTransitionReversed;

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	bool IsStateCurrent(const EAnimatedActorState DoorState) const { return StateCurrent == DoorState; }

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	bool IsStatePrevious(const EAnimatedActorState DoorState) const { return StatePrevious == DoorState; }

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	bool IsStateTarget(const EAnimatedActorState DoorState) const { return StateTarget == DoorState; }

	UFUNCTION(BlueprintImplementableEvent, Category="AnimatedActor|States")
	void OnStateChanged(const EAnimatedActorState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category="AnimatedActor|States")
	void OnTransitionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="AnimatedActor|States")
	void OnTransitionReversed();
protected:
	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	void SetState(const EAnimatedActorState NewState);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimatedActor|States", meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StateInitial = EAnimatedActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStateCurrent, Category="AnimatedActor|States",
		meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StateCurrent = EAnimatedActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStatePrevious, Category="AnimatedActor|States",
		meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StatePrevious = EAnimatedActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStateTarget, Category="AnimatedActor|States",
		meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StateTarget = EAnimatedActorState::Opened;

	void SetTargetState();

	bool CanStartAnimation() const;

	// Actions
public:
	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Actions")
	virtual void Open();

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Actions")
	virtual void Close();

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Actions")
	virtual void Lock();

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Actions")
	virtual void Unlock();

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Actions")
	virtual void Enable();

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Actions")
	virtual void Disable();

protected:
private:
};
