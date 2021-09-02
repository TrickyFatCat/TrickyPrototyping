// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/Session/SessionPlayerController.h"

#include "Core/Session/SessionGameMode.h"

ASessionPlayerController::ASessionPlayerController()
{
}

void ASessionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	
	if (World)
	{
		ASessionGameMode* GameMode = Cast<ASessionGameMode>(World->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->OnSessionStateChanged.AddUObject(this, &ASessionPlayerController::OnSessionStateChanged);
		}
	}
}

void ASessionPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASessionPlayerController::OnSessionStateChanged(const ESessionState NewState)
{
	NewState == ESessionState::Progress ? SetInputMode(FInputModeGameOnly()) : SetInputMode(FInputModeUIOnly());

	if (NewState != ESessionState::Progress)
	{
		bShowMouseCursor = true;
	}
}