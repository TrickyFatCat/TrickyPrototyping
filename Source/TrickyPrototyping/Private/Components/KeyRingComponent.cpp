// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#include "Components/KeyRingComponent.h"

UKeyRingComponent::UKeyRingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UKeyRingComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UKeyRingComponent::AddKey(const TSubclassOf<UKeyType> NewKey)
{
	if (!NewKey) return false;

	if (HasKey(NewKey)) return false;

	AcquiredKeys.AddUnique(NewKey);
	return true;
}

bool UKeyRingComponent::RemoveKey(const TSubclassOf<UKeyType> KeyType)
{
	if (!KeyType) return false;

	if (!HasKey(KeyType)) return false;

	AcquiredKeys.Remove(KeyType);
	return true;
}


bool UKeyRingComponent::HasKey(const TSubclassOf<UKeyType> KeyType) const
{
	return AcquiredKeys.Contains(KeyType);
}

