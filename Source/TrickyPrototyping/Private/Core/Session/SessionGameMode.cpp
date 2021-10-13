// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/Session/SessionGameMode.h"

#include "Core/Session/SessionHUD.h"
#include "TimerManager.h"

ASessionGameMode::ASessionGameMode()
{
	HUDClass = ASessionHUD::StaticClass();
}

void ASessionGameMode::StartPlay()
{
	Super::StartPlay();

	if (SessionData.PreparationTimerDuration <= 0.f)
	{
		StartSession();
		return;
	}

	StartPreparationTimer();
}

bool ASessionGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	if (CurrentState != ESessionState::GameOver)
	{
		SetSessionState(ESessionState::Pause);
	}
	else
	{
		return false;
	}
	
	return Super::SetPause(PC, CanUnpauseDelegate);
}

bool ASessionGameMode::ClearPause()
{
	SetSessionState(PreviousState);

	return Super::ClearPause();
}

void ASessionGameMode::SetPreparationTimerDuration(const float NewDuration)
{
	if (NewDuration < 0.f) return;

	SessionData.PreparationTimerDuration = NewDuration;
}

float ASessionGameMode::GetPreparationTimerRemainingTime()
{
	if (!GetWorld()) return -1.f;

	return GetWorld()->GetTimerManager().GetTimerRemaining(StartTimerHandle);
}

float ASessionGameMode::GetSessionRemainingTime() const
{
	if (!GetWorld()) return -1.f;

	if (SessionData.bIsSessionTimeLimited)
	{
		return GetWorld()->GetTimerManager().GetTimerRemaining(SessionTimerHandle);
	}

	return GetSessionElapsedTime();
}

float ASessionGameMode::GetSessionElapsedTime() const
{
	if (!GetWorld()) return -1.f;

	if (SessionData.bIsSessionTimeLimited)
	{
		return GetWorld()->GetTimerManager().GetTimerElapsed(SessionTimerHandle);
	}

	return GetWorld()->GetTimeSeconds() - TimeSecondsOnStartSession;
}

void ASessionGameMode::SetSessionState(const ESessionState NewState)
{
	if (CurrentState == NewState) return;

	PreviousState = CurrentState;
	CurrentState = NewState;
	OnSessionStateChanged.Broadcast(NewState);
}

void ASessionGameMode::StartPreparationTimer()
{
	if (!GetWorld()) return;

	SetSessionState(ESessionState::Preparation);
	GetWorld()->GetTimerManager().SetTimer(StartTimerHandle,
	                                       this,
	                                       &ASessionGameMode::StartSession,
	                                       SessionData.PreparationTimerDuration,
	                                       false);
}

void ASessionGameMode::StartSession()
{
	if (!GetWorld()) return;

	SetSessionState(ESessionState::Progress);

	if (SessionData.bIsSessionTimeLimited)
	{
		GetWorld()->GetTimerManager().SetTimer(SessionTimerHandle,
		                                       this,
		                                       &ASessionGameMode::FinishSession,
		                                       SessionData.SessionDuration,
		                                       false);
	}
	else
	{
		TimeSecondsOnStartSession = GetWorld()->GetTimeSeconds();
	}
}

void ASessionGameMode::FinishSession()
{
	SetSessionState(ESessionState::GameOver);
}
