// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractiveActor.generated.h"

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractiveActorChangedStateSignature, EInteractiveActorState, NewState);

UCLASS()
class TRICKYPROTOTYPING_API ABaseInteractiveActor : public AActor
{
	GENERATED_BODY()

public:
	ABaseInteractiveActor();

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
	void FillAnimatedComponents(TArray<USceneComponent*>& Components);

	virtual void StartAnimation();
	virtual void ReverseAnimation();
	virtual void StopAnimation();
	
	UFUNCTION()
	virtual void FinishAnimation();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	UCurveFloat* AnimationCurve = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	TArray<USceneComponent*> AnimatedComponents;

	UPROPERTY()
	TArray<FTransform> InitialTransforms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	TArray<FTransform> TargetTransforms;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateInitial = EInteractiveActorState::Closed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StateCurrent = EInteractiveActorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="States", meta=(AllowPrivateAccess="true"))
	EInteractiveActorState StatePrevious = EInteractiveActorState::Closed;

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
