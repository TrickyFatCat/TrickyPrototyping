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

	bShowMouseCursor = false;
}

void ASessionPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASessionPlayerController::OnSessionStateChanged(const ESessionState NewState)
{
	switch (NewState)
	{
	case ESessionState::Progress:
		bShowMouseCursor = bShowCursorOnStart;
		SetInputMode(FInputModeGameOnly());
		EnableInput(this);
		break;

	case ESessionState::GameOver:
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		DisableInput(this);
		StopMovement();
		GetPawn()->TurnOff();
		break;

	default:
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		DisableInput(this);
		break;
	}
}
