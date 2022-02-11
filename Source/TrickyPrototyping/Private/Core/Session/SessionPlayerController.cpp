// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/Session/SessionPlayerController.h"

#include "Core/Session/SessionGameMode.h"
#include "Kismet/GameplayStatics.h"

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
	SetTickableWhenPaused(true);
}

void ASessionPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASessionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputActionBinding& PauseBinding = InputComponent->BindAction("Pause",
	                                                               IE_Pressed,
	                                                               this,
	                                                               &ASessionPlayerController::PauseGame);
	PauseBinding.bExecuteWhenPaused = true;
}

void ASessionPlayerController::OnSessionStateChanged(const ESessionState NewState)
{
	auto ToggleInput = [&](const bool bEnableInput, const bool bShowCursor, const FInputModeDataBase& InputMode)
	{
		bEnableInput ? EnableInput(this) : DisableInput(this);
		bShowMouseCursor = bShowCursor;
		SetInputMode(InputMode);
	};
	
	switch (NewState)
	{
	case ESessionState::Progress:
		ToggleInput(true, bShowCursorOnStart, FInputModeGameOnly());
		break;

	case ESessionState::GameOver:
		ToggleInput(false, true, FInputModeUIOnly());
		StopMovement();
		GetPawn()->TurnOff();
		break;

	case ESessionState::Pause:
		ToggleInput(false, true, FInputModeGameAndUI());
		break;

	default:
		ToggleInput(false, true, FInputModeUIOnly());
		break;
	}
}

void ASessionPlayerController::PauseGame()
{
	IsPaused() ? UGameplayStatics::SetGamePaused(GetWorld(), false) : UGameplayStatics::SetGamePaused(GetWorld(), true);
}
