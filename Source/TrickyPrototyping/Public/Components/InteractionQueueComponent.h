// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionQueueComponent.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	/**
	 * An actor which has an interaction interface.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="InteractionData")
	AActor* Actor = nullptr;

	/**
	 * Determines if the actor required to be in line of sight for interaction.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="InteractionData")
	bool bRequireLineOfSight = false;

	/**
	 * A message which can be used in HUD to show a type of interaction.
	 */
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

	UFUNCTION(BlueprintCallable, Category="Interaction|Queue")
	FInteractionData GetFirstDataInQueue() const;

	UFUNCTION(BlueprintGetter, Category="Interaction")
	AActor* GetActorInSight() const { return ActorInSight; }

	UFUNCTION(BlueprintCallable, Category="Interaction")
	FString GetInteractionMessage() const;

private:
	/**
	 * Determine if the line of sight check is enabled.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	bool bSortByLineOfSight = false;

	/**
	 * Determines length of sphere trace for the line of sight check;
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Interaction",
		meta=(AllowPrivateAccess = "true", EditCondition="bSortByLineOfSight", ClampMin="1"))
	float SightDistance = 120.f;

	/**
	 * Determines radius of sphere trace for the line of sight check;
	 */
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
