// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActor.h"
#include "BaseDoor.generated.h"

/**
 * 
 */

UCLASS()
class TRICKYPROTOTYPING_API ABaseDoor : public AInteractiveActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

};