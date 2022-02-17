// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/SplineActor.h"

#include "Components/SplineComponent.h"


ASplineActor::ASplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	auto SetDefaultMobility = [](USceneComponent* Component) -> void
	{
		Component->SetMobility(EComponentMobility::Static);
	};

	ActorRoot = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(ActorRoot);
	SetDefaultMobility(ActorRoot);

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SetDefaultMobility(SplineComponent);
	SplineComponent->SetupAttachment(GetRootComponent());
}
