// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Objects/EntityResource.h"
#include "TimerManager.h"

void UEntityResource::SetResourceData(const FResourceData& NewResourceData)
{
	ResourceData = NewResourceData;
	ResourceData.Value = ResourceData.bUseCustomInitialValue ? ResourceData.ValueInitial : ResourceData.ValueMax;
	ResourceData.AutoIncreaseData.Time = 1 / ResourceData.AutoIncreaseData.TickFrequency;
	ResourceData.AutoDecreaseData.Time = 1 / ResourceData.AutoDecreaseData.TickFrequency;

	if (ResourceData.ValueMax <= 0)
	{
		if (ResourceData.AutoIncreaseData.bIsEnabled)
		{
			ResourceData.AutoIncreaseData.bIsEnabled = false;
		}

		if (ResourceData.AutoDecreaseData.bIsEnabled)
		{
			ResourceData.AutoDecreaseData.bIsEnabled = false;
		}
	}

	StartAutoIncrease();
	StartAutoDecrease();
}

void UEntityResource::GetResourceDataByRef(FResourceData& Data)
{
	Data = ResourceData;
}

void UEntityResource::SetValue(const float NewValue)
{
	if (NewValue < 0.f || ResourceData.Value <= 0.f || NewValue == ResourceData.Value) return;

	const float OldValue = ResourceData.Value;
	ResourceData.Value = NewValue;
	const float DeltaValue = NewValue > OldValue ? NewValue - OldValue : OldValue - NewValue;
	OnValueChanged.Broadcast(ResourceData.Value, DeltaValue);
}

void UEntityResource::DecreaseValue(const float DeltaValue)
{
	if (DeltaValue <= 0.f || GetValue() <= 0.f) return;

	ResourceData.Value = FMath::Max(ResourceData.Value - DeltaValue, 0.f);
	OnValueChanged.Broadcast(ResourceData.Value, -DeltaValue);
	StartAutoIncrease();
}

void UEntityResource::IncreaseValue(const float DeltaValue, const bool bClampToMax)
{
	if (DeltaValue <= 0.f) return;

	ResourceData.Value += DeltaValue;

	if (bClampToMax)
	{
		ResourceData.Value = FMath::Min(ResourceData.Value, ResourceData.ValueMax);
	}

	OnValueChanged.Broadcast(ResourceData.Value, DeltaValue);
	StartAutoDecrease();
}

void UEntityResource::SetValueMax(const float NewValue, const bool bClampValue)
{
	if (NewValue <= 0.f) return;

	const float OldValue = ResourceData.ValueMax;
	ResourceData.ValueMax = NewValue;
	const float DeltaValue = NewValue > OldValue ? NewValue - OldValue : OldValue - NewValue;

	if (bClampValue)
	{
		SetValue(FMath::Clamp(ResourceData.Value, 0.f, GetValueMax()));
	}

	OnValueMaxChanged.Broadcast(ResourceData.ValueMax, DeltaValue);
}

void UEntityResource::DecreaseValueMax(const float DeltaValue, const bool bClampValue)
{
	if (DeltaValue <= 0.f) return;

	ResourceData.ValueMax -= DeltaValue;

	if (bClampValue)
	{
		SetValue(FMath::Clamp(ResourceData.Value, 0.f, GetValueMax()));
	}

	OnValueMaxChanged.Broadcast(ResourceData.ValueMax, -DeltaValue);
}

void UEntityResource::IncreaseValueMax(const float DeltaValue, const bool bClampValue)
{
	if (DeltaValue <= 0.f) return;

	ResourceData.ValueMax += DeltaValue;

	if (bClampValue)
	{
		SetValue(ResourceData.ValueMax);
	}

	OnValueMaxChanged.Broadcast(ResourceData.ValueMax, DeltaValue);
}

void UEntityResource::StopTimer(FTimerHandle& TimerHandle) const
{
	if (!GetWorld()) return;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(TimerHandle))
	{
		TimerManager.ClearTimer(TimerHandle);
	}
}

void UEntityResource::ProcessAutoIncrease()
{
	IncreaseValue(ResourceData.AutoIncreaseData.TickPower, false);

	if (GetNormalizedValue() >= ResourceData.AutoIncreaseData.Threshold)
	{
		StopTimer(AutoIncreaseTimerHandle);
	}
}

void UEntityResource::StartAutoIncrease()
{
	if (!ResourceData.AutoIncreaseData.bIsEnabled || GetNormalizedValue() >= ResourceData.AutoIncreaseData.Threshold ||
		!GetWorld())
		return;

	StopTimer(AutoIncreaseTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AutoIncreaseTimerHandle,
	                                       this,
	                                       &UEntityResource::ProcessAutoIncrease,
	                                       ResourceData.AutoIncreaseData.Time,
	                                       true,
	                                       ResourceData.AutoIncreaseData.StartDelay);
}

void UEntityResource::StopAutoIncrease()
{
	if (!ResourceData.AutoIncreaseData.bIsEnabled) return;

	StopTimer(AutoIncreaseTimerHandle);
}

void UEntityResource::SetAutoIncreaseEnabled(const bool bIsEnabled)
{
	if (ResourceData.AutoIncreaseData.bIsEnabled == bIsEnabled) return;

	ResourceData.AutoIncreaseData.bIsEnabled = bIsEnabled;
	bIsEnabled ? StartAutoIncrease() : StopAutoIncrease();
}

void UEntityResource::SetAutoIncreaseFrequency(const float NewFrequency)
{
	if (NewFrequency <= 0.f) return;

	ResourceData.AutoIncreaseData.TickFrequency = NewFrequency;
	ResourceData.AutoIncreaseData.Time = 1.f / NewFrequency;
	StartAutoIncrease();
}

void UEntityResource::SetAutoIncreaseValue(const float NewValue)
{
	if (NewValue <= 0.f) return;

	ResourceData.AutoIncreaseData.TickPower = NewValue;
	StartAutoIncrease();
}

void UEntityResource::SetAutoIncreaseThreshold(const float NewThreshold)
{
	if (NewThreshold < 0.f) return;

	ResourceData.AutoIncreaseData.Threshold = NewThreshold;
	StartAutoIncrease();
}

void UEntityResource::SetAutoIncreaseStartDelay(const float NewDelay)
{
	if (NewDelay <= 0.f) return;

	ResourceData.AutoIncreaseData.StartDelay = NewDelay;
}

void UEntityResource::ProcessAutoDecrease()
{
	DecreaseValue(ResourceData.AutoDecreaseData.TickPower);

	if (GetNormalizedValue() <= ResourceData.AutoDecreaseData.Threshold)
	{
		StopTimer(AutoDecreaseTimerHandle);
	}
}

void UEntityResource::StartAutoDecrease()
{
	if (!ResourceData.AutoDecreaseData.bIsEnabled || GetNormalizedValue() <= ResourceData.AutoDecreaseData.Threshold ||
		!GetWorld())
		return;

	StopTimer(AutoDecreaseTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AutoDecreaseTimerHandle,
	                                       this,
	                                       &UEntityResource::ProcessAutoDecrease,
	                                       ResourceData.AutoDecreaseData.Time,
	                                       true,
	                                       ResourceData.AutoDecreaseData.StartDelay);
}

void UEntityResource::StopAutoDecrease()
{
	if (!ResourceData.AutoDecreaseData.bIsEnabled) return;

	StopTimer(AutoDecreaseTimerHandle);
}

void UEntityResource::SetAutoDecreaseEnabled(const bool bIsEnabled)
{
	if (ResourceData.AutoDecreaseData.bIsEnabled == bIsEnabled) return;

	ResourceData.AutoDecreaseData.bIsEnabled = bIsEnabled;
	bIsEnabled ? StartAutoDecrease() : StopAutoDecrease();
}

void UEntityResource::SetAutoDecreaseFrequency(const float NewFrequency)
{
	if (NewFrequency <= 0.f) return;

	ResourceData.AutoDecreaseData.TickFrequency = NewFrequency;
	ResourceData.AutoDecreaseData.Time = 1.f / NewFrequency;
	StartAutoDecrease();
}

void UEntityResource::SetAutoDecreaseValue(const float NewValue)
{
	if (NewValue <= 0.f) return;

	ResourceData.AutoDecreaseData.TickPower = NewValue;
	StartAutoDecrease();
}

void UEntityResource::SetAutoDecreaseThreshold(const float NewThreshold)
{
	if (NewThreshold < 0.f) return;

	ResourceData.AutoDecreaseData.Threshold = NewThreshold;
	StartAutoDecrease();
}

void UEntityResource::SetAutoDecreaseStartDelay(const float NewDelay)
{
	if (NewDelay < 0.f) return;

	ResourceData.AutoDecreaseData.StartDelay = NewDelay;
}
