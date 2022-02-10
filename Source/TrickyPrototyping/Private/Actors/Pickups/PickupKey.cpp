// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#include "Actors/Pickups/PickupKey.h"
#include "Components/KeyRingComponent.h"

bool APickupKey::ActivatePickupEffect_Implementation(AActor* TargetActor)
{
	if (!TargetActor) return false;

	UKeyRingComponent* KeyRingComponent = TargetActor->FindComponentByClass<UKeyRingComponent>();

	if (!KeyRingComponent) return false;

	return KeyRingComponent->AddKey(KeyType);
}
