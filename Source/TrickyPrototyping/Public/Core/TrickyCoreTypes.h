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
	GameOver,
	GameOverWin,
	GameOverLose
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionStateChangedSignature, ESessionState);

USTRUCT(BlueprintType)
struct FSessionGameModeData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Session", meta=(ClampMin="0", ClampMax="60"))
	float PreparationTimerDuration = 3.f; // In seconds

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Session")
	bool bIsSessionTimeLimited = false;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Session",
		meta=(ClampMin="1", EditCondition="bIsSessionTimeLimited"))
	float SessionDuration = 240.f; // In seconds
};

UENUM()
enum class ETransitionCommand : uint8
{
	Start,
	Quit
};
