// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BaseBoxTriggerComponent.generated.h"

/**
 * A base box trigger component for creating different trigger components
 */
UCLASS(ClassGroup=(TrickyPrototyping), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UBaseBoxTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UBaseBoxTriggerComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintSetter, Category="TriggerComponent")
	void SetIsEnabled(const bool Value);

	UFUNCTION(BlueprintGetter, Category="TriggerComponent")
	bool GetIsEnabled() const { return bIsEnabled; }

	UFUNCTION(BlueprintGetter, Category="TriggerComponent")
	bool GetIsActorInside() const { return bIsActorInside; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category="TriggerComponent")
	void EnableTrigger();

	virtual void EnableTrigger_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category="TriggerComponent")
	void DisableTrigger();

	virtual void DisableTrigger_Implementation();

private:
	UPROPERTY(EditDefaultsOnly,
		BlueprintSetter=SetIsEnabled,
		BlueprintGetter=GetIsEnabled,
		Category="TriggerComponent",
		meta=(AllowPrivateAccess="true"))
	bool bIsEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintGetter= GetIsActorInside, Category="TriggerComponent")
	bool bIsActorInside = false;

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                            AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex,
	                            bool bFromSweep,
	                            const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                          AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex);
};
