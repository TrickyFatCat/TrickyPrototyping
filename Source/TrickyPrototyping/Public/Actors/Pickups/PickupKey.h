// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "PickupKey.generated.h"

class UKeyType;

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API APickupKey : public APickupBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup")
	TSubclassOf<UKeyType> KeyType = nullptr;

	virtual bool ActivatePickup_Implementation(AActor* TargetActor) override;
};
