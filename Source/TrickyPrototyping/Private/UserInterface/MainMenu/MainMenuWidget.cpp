// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/TransitionScreenWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserInterface/MainMenu/SplashScreenWidget.h"
#include "Core/TrickyPrototypingGameInstance.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TransitionScreen)
	{
		TransitionScreen->OnShowed.AddDynamic(this, &UMainMenuWidget::ProcessTransition);
	}

	if (SplashScreen)
	{
		SplashScreen->OnSplashFinished.AddDynamic(this, &UMainMenuWidget::OnSplashFinished);
	}

	if (Button_StartGame)
	{
		Button_StartGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGame);
	}

	if (Button_QuitGame)
	{
		Button_QuitGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGame);
	}

	APlayerController* Controller = GetOwningPlayer();

	if (Controller)
	{
		Controller->SetInputMode(FInputModeUIOnly());
		Controller->bShowMouseCursor = false;
	}
}

void UMainMenuWidget::OnSplashFinished()
{
	if (!GetOwningPlayer()) return;

	SplashScreen->SetVisibility(ESlateVisibility::Hidden);
	GetOwningPlayer()->bShowMouseCursor = true;
}

void UMainMenuWidget::ProcessTransition()
{
	if (!GetWorld()) return;

	UTrickyPrototypingGameInstance* GameInstance = GetWorld()->GetGameInstance<UTrickyPrototypingGameInstance>();

	if (!GameInstance) return;

	switch (Command)
	{
	case EMenuCommand::Start:
		UGameplayStatics::OpenLevel(this, GameInstance->GetStartLevelName());
		break;

	case EMenuCommand::Quit:
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
		break;
	}
}

void UMainMenuWidget::OnStartGame()
{
	Command = EMenuCommand::Start;
	StartTransition();
}

void UMainMenuWidget::OnQuitGame()
{
	Command = EMenuCommand::Quit;
	StartTransition();
}

void UMainMenuWidget::StartTransition() const
{
	TransitionScreen->SetVisibility(ESlateVisibility::Visible);
	TransitionScreen->Show();

	if (GetOwningPlayer())
	{
		GetOwningPlayer()->bShowMouseCursor = false;
	}
}
