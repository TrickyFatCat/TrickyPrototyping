// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyRingComponent.generated.h"

class UKeyType;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyAddedSignature, UKeyType*, KeyType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyRemovedSignature, UKeyType*, KeyType);

UCLASS(ClassGroup=(TrickyPrototyping), meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UKeyRingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKeyRingComponent();

public:
	UPROPERTY(BlueprintAssignable, Category="KeyRing")
	FOnKeyAddedSignature OnKeyAdded;

	UPROPERTY(BlueprintAssignable, Category="KeyRing")
	FOnKeyRemovedSignature OnKeyRemoved;
	
	UFUNCTION(BlueprintCallable, Category="KeyRing")
	bool AddKey(const TSubclassOf<UKeyType> NewKey);

	UFUNCTION(BlueprintCallable, Category="KeyRing")
	bool RemoveKey(const TSubclassOf<UKeyType> KeyType);

	UFUNCTION(BlueprintPure, Category="KeyRing")
	bool HasKey(const TSubclassOf<UKeyType> KeyType) const;

	UFUNCTION(BlueprintCallable, Category="KeyRing")
	UKeyType* GetKeyByClass(TSubclassOf<UKeyType> KeyClass) const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="KeyRing", meta=(AllowPrivateAccess="true"))
	TArray<UKeyType*> AcquiredKeys;
};
