// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/MainMenu/SplashScreenWidget.h"

#include "Components/Image.h"
#include "UserInterface/TransitionScreenWidget.h"
#include "TimerManager.h"
#include "Core/TrickyGameInstance.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_CLASS(LogSplashScreen, All, All);

void USplashScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TransitionScreen)
	{
		TransitionScreen->OnHidden.AddDynamic(this, &USplashScreenWidget::StartSplashTimer);
		TransitionScreen->OnShowed.AddDynamic(this, &USplashScreenWidget::SwitchSplashScreen);

		if (SplashImages.Num() == 0)
		{
			OnSplashFinished.Broadcast();
			UE_LOG(LogSplashScreen, Error, TEXT("Splash screen images array is empty."));
			LoadMainMenu();
			return;
		}

		Image_SplashScreen->SetBrushFromTexture(SplashImages[CurrentSplashIndex], true);
		TransitionScreen->Hide();
	}
}

void USplashScreenWidget::StartSplashTimer()
{
	if (!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(SplashTimerHandle,
	                                       TransitionScreen,
	                                       &UTransitionScreenWidget::Show,
	                                       SplashDuration,
	                                       false);
}

void USplashScreenWidget::SwitchSplashScreen()
{
	if (!Image_SplashScreen) return;

	++CurrentSplashIndex;

	if (CurrentSplashIndex >= SplashImages.Num())
	{
		OnSplashFinished.Broadcast();
		LoadMainMenu();
		return;
	}

	if (SplashImages[CurrentSplashIndex])
	{
		Image_SplashScreen->SetBrushFromTexture(SplashImages[CurrentSplashIndex], true);
		OnSplashChanged.Broadcast(CurrentSplashIndex, SplashImages[CurrentSplashIndex]);
	}
	else
	{
		UE_LOG(LogSplashScreen, Error, TEXT("Splashscreen %d is nullptr. Please, set this splashscreen."), CurrentSplashIndex);
	}

	TransitionScreen->Hide();
}

void USplashScreenWidget::LoadMainMenu() const
{
	if (!GetWorld()) return;

	UTrickyGameInstance* GameInstance = GetWorld()->GetGameInstance<UTrickyGameInstance>();

	if (!GameInstance) return;

	const FName MenuLevelName = GameInstance->GetMainMenuLevelName();

	if (MenuLevelName.IsNone()) return;

	UGameplayStatics::OpenLevel(this, MenuLevelName);
}
