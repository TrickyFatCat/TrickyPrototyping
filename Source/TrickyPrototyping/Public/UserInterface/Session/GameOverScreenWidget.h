// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Session/BaseSessionScreenWidget.h"
#include "GameOverScreenWidget.generated.h"

class UButton;
class UTransitionScreenWidget;

/**
 * 
 */

UCLASS()
class TRICKYPROTOTYPING_API UGameOverScreenWidget : public UBaseSessionScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
private:

};
