// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BaseBoxTriggerComponent.generated.h"

/**
 * A base box trigger component for creating different trigger components
 */
UCLASS(Blueprintable)
class TRICKYPROTOTYPING_API UBaseBoxTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UBaseBoxTriggerComponent();

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

	UFUNCTION(BlueprintGetter, Category="Trigger")
	bool GetIsActorInside() const { return bIsActorInside; }
	
private:
	UPROPERTY(EditAnywhere, Category="Trigger", meta=(AllowPrivateAccess="true"))
	bool bIsEnabled = true;

	UPROPERTY(BlueprintGetter= GetIsActorInside, Category="Trigger")
	bool bIsActorInside = false;

	virtual void EnableTrigger();
	virtual void DisableTrigger();

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
