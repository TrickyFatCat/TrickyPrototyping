// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/Session/SessionHUD.h"

#include "Core/Session/SessionGameMode.h"
#include "UserInterface/BaseUserWidget.h"

void ASessionHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateSessionWidget(ESessionState::Preparation, PreparationWidgetClass);
	CreateSessionWidget(ESessionState::Progress, ProgressWidgetClass);
	CreateSessionWidget(ESessionState::Pause, PauseWidgetClass);
	CreateSessionWidget(ESessionState::GameOver, GameOverScreenWidgetClass);

	for (const auto SessionWidgetPair : SessionWidgets)
	{
		UBaseUserWidget* SessionWidget = SessionWidgetPair.Value;

		if (!SessionWidget) continue;

		SessionWidget->AddToViewport();
		SessionWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GetWorld())
	{
		ASessionGameMode* GameMode = Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->OnSessionStateChanged.AddUObject(this, &ASessionHUD::OnSessionStateChanged);
		}
	}
}

void ASessionHUD::OnSessionStateChanged(const ESessionState NewState)
{
	if (CurrentWidget)
	{
		CurrentWidget->Hide();
	}

	if (!SessionWidgets.Contains(NewState)) return;

	CurrentWidget = SessionWidgets[NewState];
	CurrentWidget->Show();
}

void ASessionHUD::CreateSessionWidget(const ESessionState State, const TSubclassOf<UBaseUserWidget> WidgetClass)
{
	if (!WidgetClass) return;

	SessionWidgets.Add(State, CreateWidget<UBaseUserWidget>(GetWorld(), WidgetClass));
}
