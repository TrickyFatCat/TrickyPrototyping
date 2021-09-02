// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/BaseUserWidget.h"

#include "Core/TrickyPrototypingGameInstance.h"
#include "Core/Session/SessionGameMode.h"
#include "Kismet/GameplayStatics.h"

void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBaseUserWidget::Show()
{
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(ShowAnimation);
}

void UBaseUserWidget::Hide()
{
	PlayAnimation(HideAnimation);
}

void UBaseUserWidget::OnAnimationStarted_Implementation(const UWidgetAnimation* Animation)
{
}

void UBaseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == ShowAnimation)
	{
		OnShowed.Broadcast();
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		OnHidden.Broadcast();
	}
}

ASessionGameMode* UBaseUserWidget::GetSessionGameMode() const
{
	if (!GetWorld()) return nullptr;

	return Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());
}