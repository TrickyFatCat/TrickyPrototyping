// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Objects/BaseObject.h"

UWorld* UBaseObject::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld) return nullptr;

	if (GetOuter()) return GetOuter()->GetWorld();

	return nullptr;
}

void UBaseObject::PostInitProperties()
{
	UObject::PostInitProperties();

	if (GetWorld())
	{
		BeginPlay();
	}
}
