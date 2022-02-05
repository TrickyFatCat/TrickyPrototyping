// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyRingComponent.generated.h"

class UKeyType;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRICKYPROTOTYPING_API UKeyRingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKeyRingComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="KeyRing")
	bool AddKey(const TSubclassOf<UKeyType> NewKey);

	UFUNCTION(BlueprintCallable, Category="KeyRing")
	bool RemoveKey(const TSubclassOf<UKeyType> KeyType);

	UFUNCTION(BlueprintPure, Category="KeyRing")
	bool HasKey(const TSubclassOf<UKeyType> KeyType) const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="KeyRing", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UKeyType>> AcquiredKeys;
};
