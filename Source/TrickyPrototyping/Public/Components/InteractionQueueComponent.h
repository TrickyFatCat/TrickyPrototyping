// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionQueueComponent.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="InteractionData")
	AActor* Actor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="InteractionData")
	bool bRequireLineOfSight = false;
};

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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddToQue(AActor* Actor, bool bRequireLineOfSight);
	
	void RemoveFromQueue(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool Interact();
	
	bool IsQueueEmpty() const { return InteractionQueue.Num() == 0; };

	UFUNCTION(BlueprintCallable, Category="Interaction")
	AActor* GetFirstActorInQueue() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	bool bSortByLineOfSight = false;

	UPROPERTY(EditDefaultsOnly, Category="Interaction", meta=(EditCondition="bSortByLineOfSight", ClampMin="0"))
	float SortDistance = 60.f;
	
	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TArray<FInteractionData> InteractionQueue;

	UPROPERTY()
	AActor* ActorInSight = nullptr;

	void CheckLineOfSight();

	void SortQueueByLineOfSight(FHitResult& HitResult);

	bool QueueContainsActor(const AActor* Actor) const;

	AActor* GetQueuedActor(const AActor* Actor) const;

	int32 GetInteractionDataIndex(const AActor* Actor) const;
};
