// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UImage;
class USplashScreenWidget;
class UTransitionScreenWidget;

/**
 * 
 */

UENUM()
enum class EMenuCommand : uint8
{
	Start,
	Quit
};

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
	USplashScreenWidget* SplashScreen = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UButton* Button_StartGame = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_QuitGame = nullptr;

private:
	EMenuCommand Command;

	UFUNCTION()
	void OnSplashFinished();
	
	UFUNCTION()
	void ProcessTransition();
	
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

	void StartTransition() const;
};
