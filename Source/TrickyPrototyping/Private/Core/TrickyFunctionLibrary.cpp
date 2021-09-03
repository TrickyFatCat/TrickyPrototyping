// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/TrickyFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"

FString UTrickyFunctionLibrary::ConvertTimeSeconds(const ETimeFormat ConvertMethod, const float TimeSeconds)
{
	const FString Result = UKismetStringLibrary::TimeSecondsToString(TimeSeconds);
	int32 Start = 0;
	int32 Count = 0;

	switch (ConvertMethod)
	{
	case ETimeFormat::MM_SS_MsMs:
		Count = 8;
		break;

	case ETimeFormat::MM_SS_Ms:
		Count = 7;
		break;

	case ETimeFormat::MM_SS:
		Count = 5;
		break;

	case ETimeFormat::SS_MsMs:
		Start = 3;
		Count = 5;
		break;

	case ETimeFormat::SS_Ms:
		Start = 3;
		Count = 4;
		break;

	case ETimeFormat::SS:
		Start = 3;
		Count = 2;
		break;
	}

	return UKismetStringLibrary::Mid(Result, Start, Count);
}
