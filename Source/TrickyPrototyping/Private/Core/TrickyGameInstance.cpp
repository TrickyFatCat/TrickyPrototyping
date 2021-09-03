// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/TrickyGameInstance.h"
#include "Kismet/GameplayStatics.h"

FName UTrickyGameInstance::GetStartLevelName() const
{
	if (LevelNames.Num() == 0) return NAME_None;

	return LevelNames[0];
}

FName UTrickyGameInstance::GetNextLevelName() const
{
	FName NextLevelName = NAME_None;
	const FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));

	if (CurrentLevelName.IsNone() || !LevelNames.Contains(CurrentLevelName))
	{
		NextLevelName = MainMenuLevelName;
		return NextLevelName;
	}

	for (int32 i = 0; i < LevelNames.Num(); ++i)
	{
		if (LevelNames[i] != CurrentLevelName) continue;

		if (i + 1 >= LevelNames.Num())
		{
			NextLevelName = MainMenuLevelName;
			break;
		}

		NextLevelName = LevelNames[i + 1];
		break;
	}

	return NextLevelName;
}
