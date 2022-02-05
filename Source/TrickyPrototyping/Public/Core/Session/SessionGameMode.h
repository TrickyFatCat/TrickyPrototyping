// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/TrickyCoreTypes.h"
#include "SessionGameMode.generated.h"

/**
 * 
 */

UCLASS()
class TRICKYPROTOTYPING_API ASessionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASessionGameMode();

	virtual void StartPlay() override;

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	virtual bool ClearPause() override;

	UFUNCTION(BlueprintCallable, Category="Session")
	float GetPreparationTimerDuration() const { return SessionData.PreparationTimerDuration; }

	UFUNCTION(BlueprintCallable, Category="Session")
	void SetPreparationTimerDuration(const float NewDuration);

	UFUNCTION(BlueprintCallable, Category="Session")
	float GetPreparationTimerRemainingTime();

	UFUNCTION(BlueprintCallable, Category="Session")
	float GetSessionRemainingTime() const;

	UFUNCTION(BlueprintCallable, Category="Session")
	float GetSessionElapsedTime() const;

	FOnSessionStateChangedSignature OnSessionStateChanged;

protected:
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Session", meta=(AllowPrivateAccess="true"))
	ESessionState CurrentState = ESessionState::Inactive;
	
	ESessionState PreviousState;
	
	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Session",
		meta=(AllowPrivateAccess="true"))
	FSessionGameModeData SessionData;

	float TimeSecondsOnStartSession = 0.f;

	FTimerHandle StartTimerHandle;

	FTimerHandle SessionTimerHandle;

	void SetSessionState(const ESessionState NewState);

	UFUNCTION()
	void StartPreparationTimer();

	UFUNCTION()
	void StartSession();

public:
	UFUNCTION()
	void FinishSession();
};
