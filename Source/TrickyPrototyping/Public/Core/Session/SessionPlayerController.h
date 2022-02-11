// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/TrickyCoreTypes.h"
#include "SessionPlayerController.generated.h"

/**
 * A simple player controller which works with session states.
 * It already has a pause functionality. Just add the "Pause" input action in the project settings.
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
	virtual void SetupInputComponent() override;

	/**
	 * If true, the mouse cursor will be shown when the session switch to the Progress state.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controller")
	bool bShowCursorOnStart = false;
	
	UFUNCTION()
	void OnSessionStateChanged(const ESessionState NewState);

	UFUNCTION()
	void PauseGame();
};
