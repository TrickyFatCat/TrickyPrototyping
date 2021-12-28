// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TrickyFunctionLibrary.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETimeFormat : uint8
{
	MM_SS_MsMs UMETA(DisplayName="MM:SS.MsMs"),
	MM_SS_Ms UMETA(DisplayName="MM:SS.Ms"),
	MM_SS UMETA(DisplayName="MM:SS"),
	SS_MsMs UMETA(DisplayName="SS.MsMs"),
	SS_Ms UMETA(DisplayName="SS.Ms"),
	SS UMETA(DisplayName="SS")
};
UCLASS()
class TRICKYPROTOTYPING_API UTrickyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="TrickyFunctionLibrary")
	static FString ConvertTimeSeconds(const float TimeSeconds, const ETimeFormat ConvertMethod);

	UFUNCTION(BlueprintCallable, Category="TrickyFunctionLibrary")
	static void ApproachInt32(UPARAM(ref) int32& CurrentValue, const int32 TargetValue, const int32 DeltaValue);

	UFUNCTION(BlueprintCallable, Category="TrickyFunctionLibrary")
	static void ApproachFloat(UPARAM(ref) float& CurrentValue, const float TargetValue, const float DeltaValue);

private:
	static int32 ConvertMilliseconds(const int32 InitialValue, const float Fraction);
};
