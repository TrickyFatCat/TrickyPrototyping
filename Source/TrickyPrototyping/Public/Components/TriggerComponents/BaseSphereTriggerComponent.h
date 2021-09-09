// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BaseSphereTriggerComponent.generated.h"

/**
 * A base sphere trigger component for creating different trigger components
 */
UCLASS(Blueprintable)
class TRICKYPROTOTYPING_API UBaseSphereTriggerComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UBaseSphereTriggerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Trigger")
	void SetIsEnabled(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="Trigger")
	bool GetIsEnabled() const { return bIsEnabled; }

private:
	UPROPERTY(EditAnywhere, Category="Trigger", meta=(AllowPrivateAccess="true"))
	bool bIsEnabled = true;

	virtual void EnableTrigger();
	virtual void DisableTrigger();

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
