// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActorBase.h"
#include "ButtonBase.generated.h"

/**
 * A base button class. Use it for creating different buttons and levers
 */
UCLASS()
class TRICKYPROTOTYPING_API AButtonBase : public AInteractiveActorBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
};
