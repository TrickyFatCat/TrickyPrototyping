// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/Session/BaseSessionScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Core/TrickyGameInstance.h"
#include "UserInterface/TransitionScreenWidget.h"
#include "Components/Button.h"

void UBaseSessionScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TransitionScreen)
	{
		TransitionScreen->OnShowed.AddDynamic(this, &UBaseSessionScreenWidget::OnTransitionScreenShowed);
		TransitionScreen->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Button_RestartLevel)
	{
		Button_RestartLevel->OnClicked.AddDynamic(this, &UBaseSessionScreenWidget::OnRestartPressed);
	}

	if (Button_ExitToMenu)
	{
		Button_ExitToMenu->OnClicked.AddDynamic(this, &UBaseSessionScreenWidget::OnExitPressed);
	}
}

void UBaseSessionScreenWidget::StartTransition() const
{
	TransitionScreen->Show();

	if (GetOwningPlayer())
	{
		GetOwningPlayer()->bShowMouseCursor = false;
	}
}

void UBaseSessionScreenWidget::OnTransitionScreenShowed()
{
	switch (TransitionCommand)
	{
	case ETransitionCommand::Start:
		RestartGame();
		break;

	case ETransitionCommand::Quit:
		ExitToMenu();
		break;
	}
}

void UBaseSessionScreenWidget::OnRestartPressed()
{
	TransitionCommand = ETransitionCommand::Start;
	StartTransition();
}

void UBaseSessionScreenWidget::OnExitPressed()
{
	TransitionCommand = ETransitionCommand::Quit;
	StartTransition();
}

void UBaseSessionScreenWidget::ExitToMenu() const
{
	if (!GetWorld()) return;

	UTrickyGameInstance* GameInstance = GetWorld()->GetGameInstance<UTrickyGameInstance>();

	if (!GameInstance) return;

	const FName MenuLevelName = GameInstance->GetMainMenuLevelName();

	if (MenuLevelName.IsNone()) return;

	UGameplayStatics::OpenLevel(this, MenuLevelName);
}

void UBaseSessionScreenWidget::RestartGame() const
{
	const FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}
