// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#include "Components/KeyRingComponent.h"
#include "Objects/KeyType.h"

UKeyRingComponent::UKeyRingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UKeyRingComponent::AddKey(const TSubclassOf<UKeyType> NewKey)
{
	if (!NewKey) return false;

	if (HasKey(NewKey)) return false;

	UKeyType* Key = NewObject<UKeyType>(this, *NewKey);
	AcquiredKeys.AddUnique(Key);
	OnKeyAdded.Broadcast(Key);
	return true;
}

bool UKeyRingComponent::RemoveKey(const TSubclassOf<UKeyType> KeyType)
{
	if (!KeyType) return false;

	if (!HasKey(KeyType)) return false;

	UKeyType* Key = GetKeyByClass(KeyType);

	if (!Key->GetDestroyOnUse()) return false;

	OnKeyRemoved.Broadcast(Key);
	AcquiredKeys.Remove(Key);
	Key->ConditionalBeginDestroy();
	
	return true;
}


bool UKeyRingComponent::HasKey(const TSubclassOf<UKeyType> KeyType) const
{
	const UKeyType* Key = GetKeyByClass(KeyType);

	return Key != nullptr;
}

UKeyType* UKeyRingComponent::GetKeyByClass(const TSubclassOf<UKeyType> KeyClass) const
{
	if (AcquiredKeys.Num() == 0) return nullptr;
	
	auto Iterator = [&](const UKeyType* Key) { return Key->IsA(KeyClass); };
	return *AcquiredKeys.FindByPredicate(Iterator);
}
