// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatformBase.generated.h"

class UTimelineComponent;
class UCurveFloat;

UENUM(BlueprintType)
enum class EPlatformState : uint8
{
	Idle,
	Moving,
	Waiting
};

UENUM(BlueprintType)
enum class EPlatformMovementMode : uint8
{
	Loop,
	PingPong
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeStateSignature, EPlatformState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPointReachedSignature, int32, PointIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaitStartedSingnature, int32, PointIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaitFinishedSingnature, int32, PointIndex);

UCLASS(NotBlueprintable, NotBlueprintType)
class TRICKYPROTOTYPING_API AFloatingPlatformBase : public AActor
{
	GENERATED_BODY()

public:
	AFloatingPlatformBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="FloatingPlatform")
	virtual void InitPlatform();

#pragma region Actions
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category="FloatingPlatform")
	FOnChangeStateSignature OnStateChanged;

	UPROPERTY(BlueprintAssignable, Category="FloatingPlatform")
	FOnPointReachedSignature OnPointReached;

	UPROPERTY(BlueprintAssignable, Category="FloatingPlatform")
	FOnWaitStartedSingnature OnWaitStarted;

	UPROPERTY(BlueprintAssignable, Category="FloatingPlatform")
	FOnWaitFinishedSingnature OnWaitFinished;

	/**
	 * Starts platform movement.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingPlatform")
	void StartMovement();

	/**
	 * Stops platform movement.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingPlatform")
	void StopMovement();

	/**
	 * Resumes platform movement.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingPlatform")
	void ResumeMovement();

	/**
	 * Moves platform to a certain point.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingPlatform")
	void MoveToPoint(const int32 PointIndex);
#pragma endregion

#pragma region Parameters
public:
	UFUNCTION(BlueprintGetter, Category="FloatingPlatform")
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintSetter, Category="FloatingPlatform")
	void SetSpeed(const float Value);

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* PlatformRoot = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UTimelineComponent* MovementTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="FloatingPlatform", meta=(AllowPrivateAccess="true"))
	EPlatformState State = EPlatformState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FloatingPlatform", meta=(AllowPrivateAccess="true"))
	EPlatformMovementMode MovementMode = EPlatformMovementMode::Loop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FloatingPlatform", meta=(AllowPrivateAccess="true"))
	UCurveFloat* MovementAnimationCurve = nullptr;

	/**
	 * Movement speed of the platform.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetSpeed,
		BlueprintSetter=SetSpeed,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float Speed = 300.f;

	/**
	 * If true, the platform will start moving automatically on BeginPlay.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FloatingPlatform", meta=(AllowPrivateAccess="true"))
	bool bAutoStart = false;

	/**
	 * If true, the platform will move from the last point to the first.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FloatingPlatform", meta=(AllowPrivateAccess="true"))
	bool bIsReversed = false;

	/**
	 * Index of the point from which the platform will start moving.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	int32 StartPointIndex = 0;

	/**
	 * If true, the platform will stop at points while moving.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true"))
	bool bStopAtPoints = true;

	/**
	 * If true, the platform will wait at the start point before starting movement.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true", EditCondition="bStopAtPoints"))
	bool bWaitAtStart = false;

	/**
	 * Determines how long the platform will stay at the point.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true", ClampMin="0", EditCondition="bStopAtPoints"))
	float StopWaitDuration = 3.f;

	UPROPERTY(BlueprintReadOnly, Category="FloatingPlatform", meta=(AllowPrivateAccess="true"))
	FTimerHandle StopWaitTimerHandle{};

	/**
	 * If true, the platform will automatically stop only in certain points.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true", EditCondition="bStopAtPoints"))
	bool bStopAtCertainPoints = false;

	/**
	 * A set of indexes of points where platform will stop while moving.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true", EditCondition="bStopAtCertainPoints"))
	TSet<int32> CustomPointsIndexes{};
#pragma endregion

#pragma region Movement

public:
	UFUNCTION(BlueprintCallable, Category="FloatingPlatform")
	bool IndexIsValid(const int32 Index) const;

protected:
	UPROPERTY(VisibleAnywhere, Category="FloatingPlatform|Debug")
	TArray<int32> PointsIndexes;

	UPROPERTY(VisibleAnywhere, Category="FloatingPlatform|Debug")
	float TravelTime = 1.f;

	UPROPERTY(VisibleAnywhere, Category="FloatingPlatform|Debug")
	int32 CurrentPointIndex = 0;

	UPROPERTY(VisibleAnywhere, Category="FloatingPlatform|Debug")
	int32 NextPointIndex = 0;

	virtual void CalculateTravelTime();

	virtual void FillPointIndexes();

	UFUNCTION()
	virtual void MovePlatform(const float Progress);

	UFUNCTION()
	void ContinueMovement();

	void CalculateNextPointIndex();

	void StartStopWaitTimer();

	void FinishStopTimer();

	void SetState(const EPlatformState NewState);

	void CalculateTimelinePlayRate();
#pragma endregion
};
