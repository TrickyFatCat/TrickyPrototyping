// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/TrickyFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

FString UTrickyFunctionLibrary::ConvertTimeSeconds(const float TimeSeconds, const ETimeFormat ConvertMethod)
{
	const FTimespan Timespan = UKismetMathLibrary::FromSeconds(TimeSeconds);

	FString Result = "";

	const int32 TotalMinutes = static_cast<int32>(Timespan.GetTotalMinutes());
	const int32 Seconds = Timespan.GetSeconds();
	const int32 TotalSeconds = static_cast<int32>(Timespan.GetTotalSeconds());
	const int32 Milliseconds = Timespan.GetFractionMilli();

	switch (ConvertMethod)
	{
	case ETimeFormat::MM_SS_MsMs:
		Result = FString::Printf(TEXT("%02d:%02d.%02d"),
		                             TotalMinutes,
		                             Seconds,
		                             ConvertMilliseconds(Milliseconds, 0.1f));
		break;

	case ETimeFormat::MM_SS_Ms:
		Result = FString::Printf(TEXT("%02d:%02d.%d"),
		                             TotalMinutes,
		                             Seconds,
		                             ConvertMilliseconds(Milliseconds, 0.01));
		break;

	case ETimeFormat::MM_SS:
		Result = FString::Printf(TEXT("%02d:%02d"), TotalMinutes, Seconds);
		break;

	case ETimeFormat::SS_MsMs:
		Result = FString::Printf(TEXT("%02d.%02d"), TotalSeconds, ConvertMilliseconds(Milliseconds, 0.1f));
		break;

	case ETimeFormat::SS_Ms:
		Result = FString::Printf(TEXT("%02d.%d"), TotalSeconds, ConvertMilliseconds(Milliseconds, 0.01f));
		break;

	case ETimeFormat::SS:
		Result = FString::Printf(TEXT("%02d"), TotalSeconds);
		break;
	}

	return Result;
}

int32 UTrickyFunctionLibrary::ConvertMilliseconds(const int32 InitialValue, const float Fraction)
{
	return static_cast<int32>(InitialValue * Fraction);
}
