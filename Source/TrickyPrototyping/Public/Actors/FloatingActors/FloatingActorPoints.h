// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/FloatingActorBase.h"
#include "FloatingActorPoints.generated.h"

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API AFloatingActorPoints : public AFloatingActorBase
{
	GENERATED_BODY()

protected:
	virtual void CalculateTravelTime() override;

	virtual void FillPointIndexes() override;

	virtual void RemoveInvalidCustomIndexes() override;

	virtual void MovePlatform(const float Progress) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingActor|Points", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> TargetActors;
};
