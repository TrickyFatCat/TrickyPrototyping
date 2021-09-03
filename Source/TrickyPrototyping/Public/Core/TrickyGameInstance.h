// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TrickyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API UTrickyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Levels")
	FName GetStartLevelName() const;

	UFUNCTION(BlueprintCallable, Category="Levels")
	FName GetMainMenuLevelName() const { return MainMenuLevelName; }

	UFUNCTION(BlueprintCallable, Category="Levels")
	FName GetNextLevelName() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Levels")
	FName MainMenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category="Levels")
	TArray<FName> LevelNames;
};
