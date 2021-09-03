// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Core/TrickyCoreTypes.h"
#include "UserInterface/BaseUserWidget.h"
#include "BaseSessionScreenWidget.generated.h"

class UButton;
class UTransitionScreenWidget;

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UBaseSessionScreenWidget : public UBaseUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	UTransitionScreenWidget* TransitionScreen = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_ExitToMenu = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_RestartLevel = nullptr;
	
	ETransitionCommand TransitionCommand;
	
	void StartTransition() const;
	
	UFUNCTION()
	virtual void OnTransitionScreenShowed();
	
private:
	UFUNCTION()
	void OnRestartPressed();

	UFUNCTION()
	void OnExitPressed();
	
	void ExitToMenu() const;
	void RestartGame() const;
};
