// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/BaseUserWidget.h"

void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	DefaultVisibility = GetVisibility();
}

void UBaseUserWidget::Show()
{
	PlayAnimation(ShowAnimation);
}

void UBaseUserWidget::Hide()
{
	PlayAnimation(HideAnimation);
}

void UBaseUserWidget::OnAnimationStarted_Implementation(const UWidgetAnimation* Animation)
{
	SetVisibility(DefaultVisibility);
}

void UBaseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	SetVisibility(ESlateVisibility::Hidden);
	Animation == ShowAnimation ? OnShowed.Broadcast() : OnHidden.Broadcast();
}
