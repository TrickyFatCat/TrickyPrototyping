// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionQueueComponent.generated.h"

/**
 * A component which manages interaction queue
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UInteractionQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionQueueComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddToQue(AActor* Actor);
	
	void RemoveFromQueue(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool Interact();
	
	bool IsQueueEmpty() const { return InteractionQueue.Num() == 0; };

	UFUNCTION(BlueprintCallable, Category="Interaction")
	AActor* GetTargetActor() const;

private:
	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TArray<AActor*> InteractionQueue;
};
