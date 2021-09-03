// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core/TrickyCoreTypes.h"
#include "SessionHUD.generated.h"

class UBaseUserWidget;

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API ASessionHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UBaseUserWidget> PreparationWidgetClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UBaseUserWidget> ProgressWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UBaseUserWidget> PauseWidgetClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UBaseUserWidget> GameOverScreenWidgetClass = nullptr;

private:
	UPROPERTY()
	TMap<ESessionState, UBaseUserWidget*> SessionWidgets;

	UPROPERTY()
	UBaseUserWidget* CurrentWidget = nullptr;

	UFUNCTION()
	void OnSessionStateChanged(ESessionState NewState);

	void CreateSessionWidget(const ESessionState State, TSubclassOf<UBaseUserWidget> WidgetClass);
};
