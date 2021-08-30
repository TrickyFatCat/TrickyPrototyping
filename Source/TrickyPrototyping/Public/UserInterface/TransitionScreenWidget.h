// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/BaseUserWidget.h"
#include "TransitionScreenWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UTransitionScreenWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* Image_Transition = nullptr;
};
