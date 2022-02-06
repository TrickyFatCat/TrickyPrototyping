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

	/**
	 * Determine if the resource will increase/decrease current value automatically.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category="EntityResource")
	bool bIsEnabled = false;

	/**
	 * The amount of the resource on which current value increased/decreased every tick.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="EntityResource",
		meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float TickPower = 1.f;

	/**
	 * How often the resource will increase/decrease automatically every second.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="EntityResource",
		meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float TickFrequency = 1.f;

	/**
	 * The relative value of resource after which auto increase/decrease starts.
	 * For increase current Value <= Threshold;
	 * For decrease current Value >= Threshold;
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="EntityResource",
		meta=(EditCondition="bIsEnabled", ClampMin="0.0", ClampMax="1.0"))
	float Threshold = 1.f;

	/**
	 * A delay time after which auto increase/decrease starts. If == 0, they start immediately.
	 * In seconds;
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="EntityResource",
		meta=(EditCondition="bIsEnabled", ClampMin="0.0"))
	float StartDelay = 1.f; //

	UPROPERTY()
	float Time = 1.f; // Time between ticks in seconds.
};

USTRUCT(BlueprintType)
struct FResourceData
{
	GENERATED_USTRUCT_BODY()

	/**
	 * Current Value of the resource.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EntityResource")
	float Value = 100.f;

	/**
	 * Maximum Value of te resource.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource", meta=(ClampMin="0.0"))
	float ValueMax = 100.f;

	/**
	 * If true Value = ValueInitial, else Value = ValueMax.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource")
	bool bUseCustomInitialValue = false;

	/**
	 * Determines custom Value.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="EntityResource",
		meta=(EditCondition="bUseCustomInitialValue", ClampMin="0.0"))
	float ValueInitial = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource")
	FResourceAutoChangeData AutoIncreaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResource")
	FResourceAutoChangeData AutoDecreaseData;
};

UCLASS(BlueprintType)
class TRICKYPROTOTYPING_API UEntityResource : public UBaseObject
{
	GENERATED_BODY()
	// General
public:
	/**
	 * Sets resource data and calculates some values.
	 */
	UFUNCTION(BlueprintSetter, Category="EntitiyResource")
	void SetResourceData(const FResourceData& NewResourceData);

	UFUNCTION(BlueprintGetter, Category="EntityResource")
	FResourceData GetResourceData() const { return ResourceData; }

	// Value
	/**
	 * Sets current Value of the resource.
	 * @param NewValue - a new value of current Value.
	 * @warning For calculations use DecreaseValue and IncreaseValue functions.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|CurrentValue")
	void SetValue(const float NewValue);

	/**
	 * Returns current Value of the resource.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|CurrentValue")
	float GetValue() const { return ResourceData.Value; }

	/**
	 * Decreases current Value by given DeltaValue.
	 * @param DeltaValue - the amount of the resource current Value will be decreased.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|CurrentValue")
	void DecreaseValue(const float DeltaValue);

	/**
	 * Increases current Value by given DeltaValue.
	 * @param DeltaValue - the amount of the resource current Value will be increased.
	 * @param bClampToMax - if true current Value won't increase more than ValueMax.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|CurrentValue")
	void IncreaseValue(const float DeltaValue, const bool bClampToMax = true);

	/**
	 * Returns normalized current Value.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|CurrentValue")
	float GetNormalizedValue() const { return ResourceData.Value / ResourceData.ValueMax; }

	// ValueMax
	/**
	 * Sets ValueMax.
	 * @param NewValue - a new value of ValueMax;
	 * @param bClampValue - if true, Value will be clamped between 0 and new ValueMax;
	 * @warning For calculations use <b>DecreaseValueMax</b> and **IncreaseValueMax** functions.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|MaxValue")
	void SetValueMax(const float NewValue, const bool bClampValue = false);

	/**
	 * Returns ValueMax.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|MaxValue")
	float GetValueMax() const { return ResourceData.ValueMax; }

	/**
	 * Decrease ValueMax by a given DeltaValue.
	 * @param DeltaValue - the amount of resource current ValueMax will be decreased;
	 * @param bClampValue - if true, current Value will be clamped between 0 and new ValueMax;
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|MaxValue")
	void DecreaseValueMax(const float DeltaValue, const bool bClampValue = true);
	
	/**
	 * Decrease ValueMax by a given DeltaValue.
	 * @param DeltaValue - the amount of resource current ValueMax will be increased;
	 * @param bClampValue - if true, current Value will be set to new ValueMax;
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResource|MaxValue")
	void IncreaseValueMax(const float DeltaValue, const bool bClampValue = false);

	// Delegates
	FOnValueChangedSignature OnValueChanged;

	FOnValueMaxChangedSignature OnValueMaxChanged;
protected:
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetResourceData,
		BlueprintSetter=SetResourceData,
		Category="EntityResource")
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
