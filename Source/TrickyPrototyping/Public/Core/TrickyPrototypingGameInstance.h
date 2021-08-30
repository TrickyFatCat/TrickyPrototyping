// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TrickyPrototypingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UTrickyPrototypingGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Levels")
	FName GetStartLevelName() const { return StartLevelName; }

	UFUNCTION(BlueprintCallable, Category="Levels")
	FName GetMainMenuLevelName() const { return MainMenuLevelName; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Levels")
	FName StartLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category="Levels")
	FName MainMenuLevelName = NAME_None;
};
