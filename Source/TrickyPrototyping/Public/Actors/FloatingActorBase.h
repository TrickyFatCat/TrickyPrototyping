// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActorBase.generated.h"

class UTimelineComponent;
class UCurveFloat;

UENUM(BlueprintType)
enum class EFloatingActorState : uint8
{
	Idle,
	Moving,
	Waiting
};

UENUM(BlueprintType)
enum class EFloatingActorMovementMode : uint8
{
	Manual,
	Loop,
	PingPong
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeStateSignature, EFloatingActorState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPointReachedSignature, int32, PointIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaitStartedSingnature, int32, PointIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaitFinishedSingnature, int32, PointIndex);

UCLASS(NotBlueprintable, NotBlueprintType)
class TRICKYPROTOTYPING_API AFloatingActorBase : public AActor
{
	GENERATED_BODY()

public:
	AFloatingActorBase();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category="FloatingActor")
	virtual void ConstructActor();

#pragma region Actions
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category="FloatingActor")
	FOnChangeStateSignature OnStateChanged;

	UPROPERTY(BlueprintAssignable, Category="FloatingActor")
	FOnPointReachedSignature OnPointReached;

	UPROPERTY(BlueprintAssignable, Category="FloatingActor")
	FOnWaitStartedSingnature OnWaitStarted;

	UPROPERTY(BlueprintAssignable, Category="FloatingActor")
	FOnWaitFinishedSingnature OnWaitFinished;

	/**
	 * Starts platform movement.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingActor")
	void StartMovement();

	/**
	 * Stops platform movement.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingActor")
	void StopMovement();

	/**
	 * Resumes platform movement.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingActor")
	void ResumeMovement();

	/**
	 * Moves platform to a certain point.
	 * Works only in Manual mode.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingActor")
	void MoveToPoint(const int32 PointIndex);
#pragma endregion

#pragma region Parameters
public:
	UFUNCTION(BlueprintGetter, Category="FloatingActor")
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintSetter, Category="FloatingActor")
	void SetSpeed(const float Value);

	UFUNCTION(BlueprintGetter, Category="FloatingActor")
	float GetWaitDuration() const { return WaitDuration; }

	UFUNCTION(BlueprintSetter, Category="FloatingActor")
	void SetWaitDuration(const float Value);

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* PlatformRoot = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UTimelineComponent* MovementTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="FloatingActor", meta=(AllowPrivateAccess="true"))
	EFloatingActorState State = EFloatingActorState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingActor", meta=(AllowPrivateAccess="true"))
	EFloatingActorMovementMode MovementMode = EFloatingActorMovementMode::Loop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FloatingActor", meta=(AllowPrivateAccess="true"))
	UCurveFloat* MovementAnimationCurve = nullptr;

	/**
	 * Movement speed of the platform.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetSpeed,
		BlueprintSetter=SetSpeed,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float Speed = 300.f;

	/**
	 * Index of the point from which the platform will start moving.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	int32 StartPointIndex = 0;

	/**
	 * If true, the platform will start moving automatically on BeginPlay.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", EditCondition="MovementMode!=EFloatingActorMovementMode::Manual"))
	bool bAutoStart = false;

	/**
	 * If true, the platform will move from the last point to the first.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", EditCondition="MovementMode!=EFloatingActorMovementMode::Manual"))
	bool bIsReversed = false;

	/**
	 * If true, the platform will stop at points while moving.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", EditCondition="MovementMode!=EFloatingActorMovementMode::Manual"))
	bool bStopAtPoints = true;

	/**
	 * If true, the platform will wait at the start point before starting movement.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", EditCondition=
			"bStopAtPoints && MovementMode!=EFloatingActorMovementMode::Manual"))
	bool bWaitAtStart = false;

	/**
	 * Determines how long the platform will stay at the point.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetWaitDuration,
		BlueprintSetter=SetWaitDuration,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", ClampMin="0", EditCondition=
			"bStopAtPoints && MovementMode!=EFloatingActorMovementMode::Manual"))
	float WaitDuration = 3.f;

	UPROPERTY(BlueprintReadOnly, Category="FloatingActor", meta=(AllowPrivateAccess="true"))
	FTimerHandle WaitTimerHandle{};

	/**
	 * If true, the platform will automatically stop only in certain points.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", EditCondition=
			"bStopAtPoints && MovementMode!=EFloatingActorMovementMode::Manual"))
	bool bStopAtCertainPoints = false;

	/**
	 * A set of indexes of points where platform will stop while moving.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingActor",
		meta=(AllowPrivateAccess="true", EditCondition=
			"bStopAtPoints && bStopAtCertainPoints && MovementMode!=EFloatingActorMovementMode::Manual"))
	TSet<int32> CustomStopsIndexes{};
#pragma endregion

#pragma region Movement

public:
	UFUNCTION(BlueprintCallable, Category="FloatingActor")
	bool IndexIsValid(const int32 Index) const;

protected:
	UPROPERTY(VisibleAnywhere, Category="FloatingActor|Debug")
	TArray<int32> PointsIndexes;

	UPROPERTY(VisibleAnywhere, Category="FloatingActor|Debug")
	float TravelTime = 1.f;

	UPROPERTY(VisibleAnywhere, Category="FloatingActor|Debug")
	int32 CurrentPointIndex = 0;

	UPROPERTY(VisibleAnywhere, Category="FloatingActor|Debug")
	int32 NextPointIndex = 0;

	virtual void CalculateTravelTime();

	virtual void FillPointIndexes();

	virtual void RemoveInvalidCustomIndexes();

	UFUNCTION()
	virtual void MovePlatform(const float Progress);

	UFUNCTION()
	void ContinueMovement();

	void CalculateNextPointIndex();

	void StartStopWaitTimer();

	void FinishStopTimer();

	void SetState(const EFloatingActorState NewState);
#pragma endregion
};
