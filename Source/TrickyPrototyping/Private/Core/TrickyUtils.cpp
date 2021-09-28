// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "Core/TrickyUtils.h"
#include "GameFramework/Character.h"

bool FTrickyUtils::GetPlayerViewPoint(AActor* CharacterActor, FVector& ViewLocation, FRotator& ViewRotation)
{
	ACharacter* Character = Cast<ACharacter>(CharacterActor);

	if (!Character) return false;

	if (Character->IsPlayerControlled())
	{
		const APlayerController* Controller = Character->GetController<APlayerController>();

		if (!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	return true;
	
}
