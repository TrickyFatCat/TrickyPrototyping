// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/BaseUserWidget.h"

void UBaseUserWidget::Show()
{
	PlayAnimation(ShowAnimation);
}

void UBaseUserWidget::Hide()
{
	PlayAnimation(HideAnimation);
}

void UBaseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Animation == ShowAnimation ? OnShowed.Broadcast() : OnHidden.Broadcast();
}
