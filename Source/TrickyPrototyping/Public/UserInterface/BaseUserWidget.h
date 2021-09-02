// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class ASessionGameMode;
class UTrickyPrototypingGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHiddenSignature);

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	void Show();
	
	void Hide();

	UPROPERTY(BlueprintAssignable, Category="Animation")
	FOnShowedSignature OnShowed;
	
	UPROPERTY(BlueprintAssignable, Category="Animation")
	FOnHiddenSignature OnHidden;

protected:
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* ShowAnimation = nullptr;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* HideAnimation = nullptr;

	virtual void OnAnimationStarted_Implementation(const UWidgetAnimation* Animation) override;
	
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
	ASessionGameMode* GetSessionGameMode() const;
	
	UFUNCTION()
	void ExitToMenu();

	UFUNCTION()
	void RestartGame();
};
