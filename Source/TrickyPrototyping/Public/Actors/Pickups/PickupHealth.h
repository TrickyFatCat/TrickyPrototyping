// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "PickupHealth.generated.h"

/**
 * Simple health pickup
 */
UCLASS()
class TRICKYPROTOTYPING_API APickupHealth : public APickupBase
{
	GENERATED_BODY()

protected:
	virtual bool ActivatePickup_Implementation(APawn* TargetPawn) override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true", ClampMin="0.0"))
	float HealPower = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	bool bClampHealToMaxHealth = true;
};
