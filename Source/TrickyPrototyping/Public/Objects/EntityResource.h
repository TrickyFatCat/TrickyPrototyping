// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Objects/BaseObject.h"
#include "EntityResource.generated.h"

/**
 * This object contains logic for basic entity resources such as Health Points, Armor, Stamina, etc.
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnValueChangedSignature, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueMaxChangedSignature, float);

USTRUCT(BlueprintType)
struct FResourceAutoChangeData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(
		EditAnywhere, BlueprintReadWrite, Category="EntityResource")
	bool bIsEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource", meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float TickValue = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource",meta=(EditCondition="bIsEnabled", ClampMin="0.0", ClampMax="1.0"))
	float Threshold = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource",meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float StartDelay = 1.f; // In seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource", meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float Frequency = 1.f; // Ticks in second
	UPROPERTY()
	float Time = 1.f; // Time between ticks in seconds.
};

USTRUCT(BlueprintType)
struct FResourceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EntityResource")
	float Value = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource", meta=(ClampMin="0.0"))
	float ValueMax = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource")
	bool bUseCustomInitialValue = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource", meta=(EditCondition="bUseCustomInitialValue", ClampMin="0.0"))
	float ValueInitial = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource")
	FResourceAutoChangeData AutoIncreaseData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource")
	FResourceAutoChangeData AutoDecreaseData;
};

UCLASS()
class TRICKYPROTOTYPING_API UEntityResource : public UBaseObject
{
	GENERATED_BODY()
	// General
public:
	void SetResourceData(const FResourceData& NewResourceData);
	FResourceData GetResourceData() const { return ResourceData; }

	// Value
	void SetValue(const float NewValue);
	float GetValue() const { return ResourceData.Value; }
	void DecreaseValue(const float DeltaValue);
	void IncreaseValue(const float DeltaValue, const bool bClampToMax = true);
	float GetNormalizedValue() const { return ResourceData.Value / ResourceData.ValueMax; }

	// ValueMax
	void SetValueMax(const float NewValue, const bool bClampValue = false);
	float GetValueMax() const { return ResourceData.ValueMax; }
	void DecreaseValueMax(const float DeltaValue, const bool bClampValue = true);
	void IncreaseValueMax(const float DeltaValue, const bool bClampValue = false);

	// Delegates
	FOnValueChangedSignature OnValueChanged;
	FOnValueMaxChangedSignature OnValueMaxChanged;
protected:
	FResourceData ResourceData;
	void StopTimer(FTimerHandle& TimerHandle) const;

	// Auto increase
public:
	void ProcessAutoIncrease();
	void StartAutoIncrease();
	void StopAutoIncrease();
	void SetAutoIncreaseEnabled(const bool bIsEnabled);
	void SetAutoIncreaseFrequency(const float NewFrequency);
	void SetAutoIncreaseValue(const float NewValue);
	void SetAutoIncreaseThreshold(const float NewThreshold);
	void SetAutoIncreaseStartDelay(const float NewDelay);

protected:
	FTimerHandle AutoIncreaseTimerHandle;
	
	// Auto decrease
public:
	void ProcessAutoDecrease();
	void StartAutoDecrease();
	void StopAutoDecrease();
	void SetAutoDecreaseEnabled(const bool bIsEnabled);
	void SetAutoDecreaseFrequency(const float NewFrequency);
	void SetAutoDecreaseValue(const float NewValue);
	void SetAutoDecreaseThreshold(const float NewThreshold);
	void SetAutoDecreaseStartDelay(const float NewDelay);
	
private:
	FTimerHandle AutoDecreaseTimerHandle;
};
