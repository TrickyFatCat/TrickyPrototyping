// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

class FTrickyUtils
{
public:
	static bool GetPlayerViewPoint(AActor* CharacterActor, FVector& ViewLocation, FRotator& ViewRotation);

	template <typename T>
	static void Approach(T& CurrentValue, const T TargetValue, const T DeltaValue)
	{
		CurrentValue = TargetValue > CurrentValue
			               ? FMath::Min(CurrentValue + DeltaValue, TargetValue)
			               : FMath::Max(CurrentValue - DeltaValue, TargetValue);
	}
};
