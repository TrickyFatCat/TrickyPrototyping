// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Objects/BaseObject.h"
#include "EntityResource.generated.h"

/**
 * This object contains logic for basic entity resources such as Health Points, Armor, Stamina, etc.
 */

USTRUCT(BlueprintType)
struct FResourceAutoChangeData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float TickValue = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bIsEnabled", ClampMin="0.0", ClampMax="1.0"))
	float Threshold = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float StartDelay = 1.f; // In seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float Frequency = 1.f;
	UPROPERTY()
	float Time = 1.f; // Time between ticks in seconds.
};

USTRUCT(BlueprintType)
struct FResourceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Value = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"))
	float ValueMax = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCustomInitialValue = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bUseCustomInitialValue", ClampMin="0.0"))
	float ValueInitial = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FResourceAutoChangeData AutoIncreaseData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FResourceAutoChangeData AutoDecreaseData;
};

UCLASS()
class TRICKYPROTOTYPING_API UEntityResource : public UBaseObject
{
	GENERATED_BODY()
};
