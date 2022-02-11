// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Objects/BaseObject.h"
#include "KeyType.generated.h"

/**
 * A simple key object class to create different keys.
 * @warning this class does not spawn in runtime.
 */
UCLASS(Blueprintable, BlueprintType)
class TRICKYPROTOTYPING_API UKeyType : public UBaseObject
{
	GENERATED_BODY()

public:
	bool GetDestroyOnUse() const { return bDestroyOnUse; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Key")
	bool bDestroyOnUse = false;
};
