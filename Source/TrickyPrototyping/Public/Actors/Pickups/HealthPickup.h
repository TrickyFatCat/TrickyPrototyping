// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/BasePickup.h"
#include "HealthPickup.generated.h"

/**
 * Simple health pickup
 */
UCLASS()
class TRICKYPROTOTYPING_API AHealthPickup : public ABasePickup
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true", ClampMin="0.0"))
	float HealPower = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	bool bClampHealToMaxHealth = true;

	virtual bool ActivatePickup(APawn* TargetPawn) override;
	
};
