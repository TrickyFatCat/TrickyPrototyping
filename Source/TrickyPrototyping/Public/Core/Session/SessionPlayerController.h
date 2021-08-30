// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/TrickyCoreTypes.h"
#include "SessionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TRICKYPROTOTYPING_API ASessionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASessionPlayerController();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	UFUNCTION()
	void OnSessionStateChanged(const ESessionState NewState);
};
