// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

class USplineComponent;

/**
 * A simple actor with spline component.
 * Great to use with FloatingActorSpline class.
 */
UCLASS(BlueprintType, Blueprintable)
class TRICKYPROTOTYPING_API ASplineActor : public AActor
{
	GENERATED_BODY()

public:
	ASplineActor();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* ActorRoot = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USplineComponent* SplineComponent = nullptr;
};
