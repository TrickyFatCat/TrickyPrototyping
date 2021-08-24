// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionQueManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UInteractionQueManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionQueManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddToQue(AActor* Actor);
	void RemoveFromQueue(AActor* Actor);
	bool Interact() const;
	bool IsQueueEmpty() const { return InteractionQueue.Num() == 0; };

	UFUNCTION(BlueprintCallable, Category="Interaction")
	AActor* GetTargetActor() const;

private:
	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TArray<AActor*> InteractionQueue;
};
