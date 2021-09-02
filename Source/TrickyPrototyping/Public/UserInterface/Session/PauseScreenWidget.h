// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Session/BaseSessionScreenWidget.h"
#include "PauseScreenWidget.generated.h"

class UButton;
class UTransitionScreenWidget;

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UPauseScreenWidget : public UBaseSessionScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_ResumeGame = nullptr;

private:
	UFUNCTION()
	void OnResumePressed();
};
