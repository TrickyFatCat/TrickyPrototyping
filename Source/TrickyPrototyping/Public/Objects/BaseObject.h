// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseObject.generated.h"

/**
 *  Base object from which other objects inherited
 */
UCLASS(Blueprintable)
class TRICKYPROTOTYPING_API UBaseObject : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintPure, Category="BaseObject")
	AActor* GetOwner() const { return Cast<AActor>(GetOuter()); }

protected:
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintImplementableEvent, Category="BaseObject")
	void BeginPlay();
};
