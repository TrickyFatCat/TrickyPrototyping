// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/Session/PauseScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_ResumeGame)
	{
		Button_ResumeGame->OnClicked.AddDynamic(this, &UPauseScreenWidget::OnResumePressed);
	}
}

void UPauseScreenWidget::OnResumePressed()
{
	UGameplayStatics::SetGamePaused(this, false);
}
