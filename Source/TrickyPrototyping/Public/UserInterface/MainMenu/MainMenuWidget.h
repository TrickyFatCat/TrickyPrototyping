// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/TrickyCoreTypes.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UImage;
class USplashScreenWidget;
class UTransitionScreenWidget;

/**
 * 
 */



UCLASS()
class TRICKYPROTOTYPING_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTransitionScreenWidget* TransitionScreen = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_StartGame = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_QuitGame = nullptr;

private:
	ETransitionCommand Command = ETransitionCommand::None;

	UFUNCTION()
	void ProcessTransition();
	
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

	void StartTransition() const;
};
