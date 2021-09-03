// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/Session/GameOverScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Core/TrickyGameInstance.h"


void UGameOverScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_NextLevel)
	{
		Button_NextLevel->OnClicked.AddDynamic(this, &UGameOverScreenWidget::OnNextLevelPressed);
	}
}

void UGameOverScreenWidget::OnTransitionScreenShowed()
{
	Super::OnTransitionScreenShowed();

	if (TransitionCommand == ETransitionCommand::NextLevel)
	{
		OpenNextLevel();
	}
}

void UGameOverScreenWidget::OnNextLevelPressed()
{
	TransitionCommand = ETransitionCommand::NextLevel;
	StartTransition();
}

void UGameOverScreenWidget::OpenNextLevel() const
{
	if (!GetWorld()) return;

	UTrickyGameInstance* GameInstance = GetWorld()->GetGameInstance<UTrickyGameInstance>();

	if (!GameInstance) return;

	const FName NextLevelName = GameInstance->GetNextLevelName();

	if (NextLevelName.IsNone()) return;

	UGameplayStatics::OpenLevel(this, NextLevelName);
}
