// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/MainMenu/SplashScreenWidget.h"

#include "Components/Image.h"
#include "UserInterface/TransitionScreenWidget.h"
#include "TimerManager.h"

void USplashScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TransitionScreen)
	{
		TransitionScreen->OnHidden.AddDynamic(this, &USplashScreenWidget::StartSplashTimer);
		TransitionScreen->OnShowed.AddDynamic(this, &USplashScreenWidget::SwitchSplashScreen);
		TransitionScreen->Hide();
	}
}

void USplashScreenWidget::StartSplashTimer()
{
	if (!GetWorld()) return;

	if (Image_TeamSplash->GetVisibility() == ESlateVisibility::Hidden)
	{
		OnSplashFinished.Broadcast();
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(SplashTimerHandle,
	                                       TransitionScreen,
	                                       &UTransitionScreenWidget::Show,
	                                       SplashDuration,
	                                       false);
}

void USplashScreenWidget::SwitchSplashScreen()
{
	if (!Image_JamSplash || !Image_TeamSplash) return;

	if (Image_JamSplash->GetVisibility() != ESlateVisibility::Hidden)
	{
		Image_JamSplash->SetVisibility(ESlateVisibility::Hidden);
		TransitionScreen->Hide();
		return;
	}

	if (Image_TeamSplash->GetVisibility() != ESlateVisibility::Hidden)
	{
		Image_TeamSplash->SetVisibility(ESlateVisibility::Hidden);
		TransitionScreen->Hide();
	}
}
