// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/MainMenu/MainMenuHUD.h"

#include "Blueprint/UserWidget.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		UUserWidget* MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
}
