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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="InteractionData")
	FString InteractionMessage = "";
};

/**
 * A component which manages interaction queue
 */

UCLASS(ClassGroup=(TrickyPrototyping), meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UInteractionQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionQueueComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void AddToQueue(AActor* Actor, const bool bRequireLineOfSight, const FString& InteractionMessage);

	void RemoveFromQueue(const AActor* Actor);

	/**
	 * Call ProcessInteraction function inside a target actor.
	 */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool Interact();

	UFUNCTION(BlueprintCallable, Category="Interaction|Queue")
	bool IsQueueEmpty() const { return InteractionQueue.Num() == 0; };

	UFUNCTION(BlueprintCallable, Category="Interaction|Queue")
	AActor* GetFirstActorInQueue() const;

	UFUNCTION(BlueprintCallable, Category="Interaction")
	FInteractionData GetFirstDataInQueue() const;

	UFUNCTION(BlueprintGetter, Category="Interaction")
	AActor* GetActorInSight() const { return ActorInSight; }

	UFUNCTION(BlueprintCallable, Category="Interaction")
	FString GetInteractionMessage() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	bool bSortByLineOfSight = false;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Interaction",
		meta=(AllowPrivateAccess = "true", EditCondition="bSortByLineOfSight", ClampMin="1"))
	float SightDistance = 120.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess="true", EditCondition="bSortByLineOfSight", ClampMin="1"))
	float SightRadius = 60.f;

	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TArray<FInteractionData> InteractionQueue;

	UPROPERTY(BlueprintGetter=GetActorInSight, Category="Interaction")
	AActor* ActorInSight = nullptr;

	void CheckLineOfSight();

	void SortQueueByLineOfSight(FHitResult& HitResult);

	bool QueueContainsActor(const AActor* Actor) const;

	AActor* GetQueuedActor(const AActor* Actor) const;

	int32 GetInteractionDataIndex(const AActor* Actor) const;
};
