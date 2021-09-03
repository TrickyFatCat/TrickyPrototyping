// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/Session/PreparationScreenWidget.h"
#include "Core/Session/SessionGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "UserInterface/TransitionScreenWidget.h"

void UPreparationScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (TransitionScreen)
	{
		TransitionScreen->Hide();
	}
}

int32 UPreparationScreenWidget::GetPreparationRemainingTime() const
{
	ASessionGameMode* GameMode = GetSessionGameMode();

	if (!GameMode) return -1.f;

	return FMath::CeilToInt(GameMode->GetPreparationTimerRemainingTime());
}
