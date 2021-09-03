// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/BaseUserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "PreparationScreenWidget.generated.h"

class UTextBlock;
class UTransitionScreenWidget;

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UPreparationScreenWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
protected:	
	UPROPERTY(meta=(BindWidget))
	UTransitionScreenWidget* TransitionScreen = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_PreparationTimer = nullptr;
	
	UFUNCTION(BlueprintPure, Category="UI")
	FTimespan GetPreparationRemainingTime() const;
};
