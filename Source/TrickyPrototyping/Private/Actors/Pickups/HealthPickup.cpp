// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/Pickups/HealthPickup.h"

#include "Components/DamageControllerComponent.h"

bool AHealthPickup::ActivatePickup(APawn* TargetPawn)
{
	if (!IsValid(TargetPawn)) return false;

	UDamageControllerComponent* DamageController = TargetPawn->FindComponentByClass<UDamageControllerComponent>();

	if (!DamageController) return false;

	DamageController->IncreaseHealth(HealPower, bClampHealToMaxHealth);
	
	return true;
}
