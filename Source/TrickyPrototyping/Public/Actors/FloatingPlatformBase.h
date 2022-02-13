// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

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
	Paused
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
	 * Pauses platform movement.
	 */
	UFUNCTION(BlueprintCallable, Category="FloatingPlatform")
	void PauseMovement();

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
	UFUNCTION(BlueprintSetter, Category="FloatingPlatform")
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintGetter, Category="FloatingPlatform")
	void SetSpeed(const float Value);
private:
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FloatingPlatform", meta=(AllowPrivateAccess="true", ClampMin="0"))
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
	 * Determines how long the platform will stay at the point.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="FloatingPlatform",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float StopDuration = 3.f;

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
		meta=(AllowPrivateAccess="true", EditCondition="bUseCustomStops"))
	TSet<int32> CustomPointsIndexes{};
#pragma endregion

#pragma region Movement

private:
	TArray<int32> PointsIndexes;

	int32 CurrentPointIndex = 0;
	int32 NextPointIndex = 1;
#pragma endregion
};
