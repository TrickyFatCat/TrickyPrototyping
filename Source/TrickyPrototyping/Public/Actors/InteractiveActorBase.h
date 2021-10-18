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
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	bool bAnimateRotation = false;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bAnimateRotation"))
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	bool bAnimateScale = false;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bAnimateScale"))
	FVector ScaleOffset = FVector::ZeroVector;
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
	UFUNCTION(BlueprintGetter, Category="InteractiveActor|Animation")
	float GetAnimationDuration() const { return AnimationDuration; }

	UFUNCTION(BlueprintSetter, Category="InteractiveActor|Animation")
	void SetAnimationDuration(const float Value);

	UFUNCTION(BlueprintGetter, Category="InteractiveActor|Animation")
	bool GetIsReversible() const { return bIsReversible; }

	UFUNCTION(BlueprintSetter, Category="InteractiveActor|Animation")
	void SetIsReversible(const bool Value) { bIsReversible = Value; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UTimelineComponent* AnimationTimeline = nullptr;

	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Animation")
	void AddAnimatedComponent(USceneComponent* NewComponent);

	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Animation")
	void FillAnimatedComponents(TArray<USceneComponent*> Components);

	virtual void StartAnimation();
	
	virtual void ReverseAnimation();
	
	virtual void StopAnimation();

	UFUNCTION()
	virtual void FinishAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractiveActor|Animation", meta=(AllowPrivateAccess="true"))
	UCurveFloat* AnimationCurve = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="InteractiveActor|Animation", meta=(AllowPrivateAccess="true"))
	TArray<USceneComponent*> AnimatedComponents;

	UPROPERTY()
	TArray<FTransform> InitialTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractiveActor|Animation", meta=(AllowPrivateAccess="true"))
	TArray<FInteractiveActorData> TransformOffsets;

	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetAnimationDuration,
		BlueprintSetter=SetAnimationDuration,
		Category="InteractiveActor|Animation",
		meta=(AllowPrivateAccess="true"))
	float AnimationDuration = 1.f;

	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetIsReversible,
		BlueprintSetter=SetIsReversible,
		Category="InteractiveActor|Animation",
		meta=(AllowPrivateAccess="true"))
	bool bIsReversible = false;

	UFUNCTION(BlueprintCallable, Category="Animation")
	void CalculatePlayRate();

	UFUNCTION()
	void AnimateTransform(const float AnimationProgress);

	bool CanBeReversed() const;

	// States
public:
	UFUNCTION(BlueprintGetter, Category="InteractiveActor|States")
	EInteractiveActorState GetStateCurrent() const { return StateCurrent; }

	UFUNCTION(BlueprintGetter, Category="InteractiveActor|States")
	EInteractiveActorState GetStateTarget() const { return StateTarget; }

	UFUNCTION(BlueprintGetter, Category="InteractiveActor|States")
	EInteractiveActorState GetStatePrevious() const { return StatePrevious; }

	UPROPERTY(BlueprintAssignable, Category="InteractiveActor|States")
	FOnInteractiveActorChangedStateSignature OnActorChangedState;

	UPROPERTY(BlueprintAssignable, Category="InteractiveActor|States")
	FOnTransitionStartedSignature OnActorTransitionStarted;

	UPROPERTY(BlueprintAssignable, Category="InteractiveActor|States")
	FOnTransitionReversedSignature OnActorTransitionReversed;

	UFUNCTION(BlueprintCallable, Category="InteractiveActor|States")
	bool IsStateCurrent(const EInteractiveActorState DoorState) const { return StateCurrent == DoorState; }

	UFUNCTION(BlueprintCallable, Category="InteractiveActor|States")
	bool IsStatePrevious(const EInteractiveActorState DoorState) const { return StatePrevious == DoorState; }

	UFUNCTION(BlueprintCallable, Category="InteractiveActor|States")
	bool IsStateTarget(const EInteractiveActorState DoorState) const { return StateTarget == DoorState; }

	UFUNCTION(BlueprintImplementableEvent, Category="InteractiveActor|States")
	void OnStateChanged(const EInteractiveActorState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category="InteractiveActor|States")
	void OnTransitionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="InteractiveActor|States")
	void OnTransitionReversed();
protected:
	UFUNCTION(BlueprintCallable, Category="InteractiveActor|States")
	void SetState(const EInteractiveActorState NewState);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractiveActor|States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateInitial = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStateCurrent, Category="InteractiveActor|States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateCurrent = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStatePrevious, Category="InteractiveActor|States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StatePrevious = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStateTarget, Category="InteractiveActor|States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateTarget = EInteractiveActorState::Opened;

	void SetTargetState();
	
	bool CanStartAnimation() const;

	// Actions
public:
	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Actions")
	virtual void Open();
	
	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Actions")
	virtual void Close();
	
	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Actions")
	virtual void Lock();
	
	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Actions")
	virtual void Unlock();
	
	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Actions")
	virtual void Enable();
	
	UFUNCTION(BlueprintCallable, Category="InteractiveActor|Actions")
	virtual void Disable();

protected:
private:
};
