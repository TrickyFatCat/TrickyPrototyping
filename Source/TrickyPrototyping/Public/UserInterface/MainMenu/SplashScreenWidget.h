// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SplashScreenWidget.generated.h"

class UImage;
class UTransitionScreenWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSplashFinishedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSplashChangedSignature,
                                             const int32,
                                             SplashIndex,
                                             const UTexture2D*,
                                             SplashImage);

/**
 * This widget is for showing simple splash screens after starting the game.
 */
UCLASS()
class TRICKYPROTOTYPING_API USplashScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="SplashScreen")
	FOnSplashFinishedSignature OnSplashFinished;

	UPROPERTY(BlueprintAssignable, Category="SplashScreen")
	FOnSplashChangedSignature OnSplashChanged;

protected:
	UPROPERTY(meta=(BindWidget))
	UTransitionScreenWidget* TransitionScreen;

	UPROPERTY(meta=(BindWidget))
	UImage* Image_SplashScreen = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="SplashScreen")
	TArray<UTexture2D*> SplashImages;

	int32 CurrentSplashIndex = 0;

	virtual void NativeOnInitialized() override;

private:
	/**
	 * How long the current splash screen will be shown.
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="SplashScreen",
		meta=(AllowPrivateAccess="true", ClampMin="0.5"))
	float SplashDuration = 1.5f;

	FTimerHandle SplashTimerHandle;

	UFUNCTION()
	void StartSplashTimer();

	UFUNCTION()
	void SwitchSplashScreen();

	UFUNCTION()
	void LoadMainMenu() const;
};
