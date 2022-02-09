// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/TransitionScreenWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserInterface/MainMenu/SplashScreenWidget.h"
#include "Core/TrickyGameInstance.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TransitionScreen)
	{
		TransitionScreen->OnShowed.AddDynamic(this, &UMainMenuWidget::ProcessTransition);
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

	TransitionScreen->SetVisibility(ESlateVisibility::Visible);
	TransitionScreen->Hide();
}

void UMainMenuWidget::ProcessTransition()
{
	if (!GetWorld()) return;

	UTrickyGameInstance* GameInstance = GetWorld()->GetGameInstance<UTrickyGameInstance>();

	if (!GameInstance) return;

	switch (Command)
	{
	case ETransitionCommand::Start:
		UGameplayStatics::OpenLevel(this, GameInstance->GetStartLevelName());
		break;

	case ETransitionCommand::Quit:
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
		break;

	default:
		if (GetOwningPlayer())
		{
			GetOwningPlayer()->bShowMouseCursor = true;
		}
		break;
	}
}

void UMainMenuWidget::OnStartGame()
{
	Command = ETransitionCommand::Start;
	StartTransition();
}

void UMainMenuWidget::OnQuitGame()
{
	Command = ETransitionCommand::Quit;
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
