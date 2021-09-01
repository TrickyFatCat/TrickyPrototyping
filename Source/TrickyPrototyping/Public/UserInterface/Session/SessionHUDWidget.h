// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/BaseUserWidget.h"
#include "SessionHUDWidget.generated.h"

class UTransitionScreenWidget;
class UTextBlock;
class ASessionGameMode;

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API USessionHUDWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	UTransitionScreenWidget* TransitionScreen = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_SessionTimer = nullptr;
	
	UFUNCTION(BlueprintPure, Category="UI")
	float GetSessionTime() const;
};
