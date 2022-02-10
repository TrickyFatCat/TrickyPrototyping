// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/TrickyFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/TrickyUtils.h"

FString UTrickyFunctionLibrary::ConvertTimeSeconds(const float TimeSeconds, const ETimeFormat TimeFormat)
{
	const FTimespan Timespan = UKismetMathLibrary::FromSeconds(TimeSeconds);

	FString Result = "";

	const int32 TotalMinutes = static_cast<int32>(Timespan.GetTotalMinutes());
	const int32 Seconds = Timespan.GetSeconds();
	const int32 TotalSeconds = static_cast<int32>(Timespan.GetTotalSeconds());
	const int32 Milliseconds = Timespan.GetFractionMilli();

	auto ConvertMilliseconds = [&Milliseconds](const float Fraction) -> int32
	{
		return static_cast<int32>(Milliseconds * Fraction);
	};

	switch (TimeFormat)
	{
		case ETimeFormat::MM_SS_MsMs:
			Result = FString::Printf(TEXT("%02d:%02d.%02d"),
			                         TotalMinutes,
			                         Seconds,
			                         ConvertMilliseconds(0.1f));
			break;

		case ETimeFormat::MM_SS_Ms:
			Result = FString::Printf(TEXT("%02d:%02d.%d"),
			                         TotalMinutes,
			                         Seconds,
			                         ConvertMilliseconds(0.01f));
			break;

		case ETimeFormat::MM_SS:
			Result = FString::Printf(TEXT("%02d:%02d"), TotalMinutes, Seconds);
			break;

		case ETimeFormat::SS_MsMs:
			Result = FString::Printf(TEXT("%02d.%02d"), TotalSeconds, ConvertMilliseconds(0.1f));
			break;

		case ETimeFormat::SS_Ms:
			Result = FString::Printf(TEXT("%02d.%d"), TotalSeconds, ConvertMilliseconds(0.01f));
			break;

		case ETimeFormat::SS:
			Result = FString::Printf(TEXT("%02d"), TotalSeconds);
			break;
	}

	return Result;
}

void UTrickyFunctionLibrary::ApproachInt32(int32& CurrentValue, const int32 TargetValue, const int32 DeltaValue)
{
	FTrickyUtils::Approach<int32>(CurrentValue, TargetValue, DeltaValue);
}

void UTrickyFunctionLibrary::ApproachFloat(float& CurrentValue, const float TargetValue, const float DeltaValue)
{
	FTrickyUtils::Approach<float>(CurrentValue, TargetValue, DeltaValue);
}