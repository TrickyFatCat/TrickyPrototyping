// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseInteractiveActor.h"
#include "BaseDoor.generated.h"

/**
 * A base door class
 */

UCLASS()
class TRICKYPROTOTYPING_API ABaseDoor : public ABaseInteractiveActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

};
