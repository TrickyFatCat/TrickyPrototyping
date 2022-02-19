// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "TrickyCoreTypes.generated.h"

UENUM(BlueprintType)
enum class ESessionState : uint8
{
	Inactive,
	Preparation,
	Progress,
	Pause,
	GameOver
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionStateChangedSignature, ESessionState, NewState);

USTRUCT(BlueprintType)
struct FSessionGameModeData
{
	GENERATED_USTRUCT_BODY()

	/**
	 * Duration of the start timer.
	 * Player can't control character while the start timer is active.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Session", meta=(ClampMin="0", ClampMax="60"))
	float PreparationTimerDuration = 3.f; // In seconds

	/**
	 * If true, the time in the chosen level will be limited.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Session")
	bool bIsSessionTimeLimited = false;

	/**
	 * Duration of the level timer.
	 * When it finishes, the session state will be changed automatically to GameOver.
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Session",
		meta=(ClampMin="1", EditCondition="bIsSessionTimeLimited"))
	float SessionDuration = 240.f; // In seconds
};

UENUM(BlueprintType)
enum class ETransitionCommand : uint8
{
	Start,
	NextLevel,
	Quit,
	None
};
