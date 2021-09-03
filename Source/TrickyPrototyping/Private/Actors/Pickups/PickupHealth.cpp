// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/Pickups/PickupHealth.h"

#include "Components/DamageControllerComponent.h"

bool APickupHealth::ActivatePickup_Implementation(APawn* TargetPawn)
{
	if (!IsValid(TargetPawn)) return false;

	UDamageControllerComponent* DamageController = TargetPawn->FindComponentByClass<UDamageControllerComponent>();

	if (!DamageController) return false;

	DamageController->IncreaseHealth(HealPower, bClampHealToMaxHealth);
	
	return true;
}
