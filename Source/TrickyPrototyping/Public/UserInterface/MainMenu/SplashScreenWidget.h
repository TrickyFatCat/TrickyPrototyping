// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SplashScreenWidget.generated.h"

class UImage;
class UTransitionScreenWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSplashFinishedSignature);

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API USplashScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Animation")
	FOnSplashFinishedSignature OnSplashFinished;

protected:
	UPROPERTY(meta=(BindWidget))
	UTransitionScreenWidget* TransitionScreen;
	
	UPROPERTY(meta=(BindWidget))
	UImage* Image_JamSplash = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UImage* Image_TeamSplash = nullptr;

	virtual void NativeOnInitialized() override;

private:
	const float SplashDuration = 1.5f;
	FTimerHandle SplashTimerHandle;

	UFUNCTION()
	void StartSplashTimer();

	UFUNCTION()
	void SwitchSplashScreen();
};
