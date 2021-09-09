// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/Pickups/PickupHealth.h"

#include "Components/DamageControllerComponent.h"

void APickupHealth::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!TargetActor) return;

	UDamageControllerComponent* DamageController = TargetActor->FindComponentByClass<UDamageControllerComponent>();

	if (!DamageController) return;

	DamageController->IncreaseHealth(HealPower, bClampHealToMaxHealth);
}
