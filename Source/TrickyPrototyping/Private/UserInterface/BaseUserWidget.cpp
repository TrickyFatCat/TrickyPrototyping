// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/BaseUserWidget.h"

#include "Core/Session/SessionGameMode.h"

void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBaseUserWidget::Show()
{
	PlayAnimation(ShowAnimation, CalculateStartTime(HideAnimation, ShowAnimation));
}

void UBaseUserWidget::Hide()
{
	PlayAnimation(HideAnimation, CalculateStartTime(ShowAnimation, HideAnimation));
}

void UBaseUserWidget::OnAnimationStarted_Implementation(const UWidgetAnimation* Animation)
{
	SetVisibility(ESlateVisibility::Visible);
}

void UBaseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == ShowAnimation)
	{
		SetVisibility(ESlateVisibility::Visible); // In case, if the player starts pressing the "Pause" action rapidly.
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

float UBaseUserWidget::CalculateStartTime(const UWidgetAnimation* CurrentAnimation, const UWidgetAnimation* NewAnimation) const
{
	const float AnimationEndTime = CurrentAnimation->GetEndTime();
	const float TargetTime = ((AnimationEndTime - GetAnimationCurrentTime(CurrentAnimation))/AnimationEndTime) * NewAnimation->GetEndTime();
	return TargetTime * static_cast<float>(IsAnimationPlaying(CurrentAnimation));
}