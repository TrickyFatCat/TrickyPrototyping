// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingPlatformBase.h"

#include "Components/TimelineComponent.h"


AFloatingPlatformBase::AFloatingPlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformRoot = CreateDefaultSubobject<USceneComponent>("PlatformRoot");
	SetRootComponent(PlatformRoot);

	MovementTimeline = CreateDefaultSubobject<UTimelineComponent>("MovementTimeline");
}

void AFloatingPlatformBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFloatingPlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloatingPlatformBase::StartMovement()
{
}

void AFloatingPlatformBase::StopMovement()
{
}

void AFloatingPlatformBase::PauseMovement()
{
}

void AFloatingPlatformBase::ResumeMovement()
{
}

void AFloatingPlatformBase::MoveToPoint(const int32 PointIndex)
{
}

void AFloatingPlatformBase::SetSpeed(const float Value)
{
	if (Value < 0.f) return;

	Speed = Value;

	// TODO Recalculate movement timers;
}

