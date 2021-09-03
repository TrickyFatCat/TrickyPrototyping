// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/BasePickup.h"
#include "Components/TriggerComponents/InteractionSphereComponent.h"

ABasePickup::ABasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionTrigger = CreateDefaultSubobject<UInteractionSphereComponent>("InteractionTrigger");
	SetRootComponent(InteractionTrigger);

	MeshScene = CreateDefaultSubobject<USceneComponent>("MeshScene");
	MeshScene->SetupAttachment(GetRootComponent());
}

void ABasePickup::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = MeshScene->GetRelativeLocation();
	InteractionTrigger->SetIsNormalTrigger(bRequireInteraction);
	
	if (bRequireInteraction)
	{
		InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnTriggerBeginOverlap);
	}
}

void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimatePosition();
	AnimateRotation();
}

bool ABasePickup::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController || !PlayerController->GetPawn() || !bRequireInteraction) return false;
	
	return ActivatePickup(PlayerController->GetPawn());
}

void ABasePickup::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!bRequireInteraction || !IsValid(OtherActor)) return;

	APawn* TargetPawn = Cast<APawn>(OtherActor);

	if (!TargetPawn) return;

	ActivatePickup(TargetPawn);
}

bool ABasePickup::ActivatePickup_Implementation(APawn* TargetPawn)
{
	return false;
}

void ABasePickup::AnimateRotation() const
{
	if (!bAnimateRotation) return;

	MeshScene->AddRelativeRotation(FRotator(0.f, RotationSpeed, 0.f));
}

void ABasePickup::AnimatePosition() const
{
	if (!bAnimatePosition || !GetWorld()) return;

	const float Time = GetWorld()->GetTimeSeconds();
	FVector CurrentLocation = MeshScene->GetRelativeLocation();
	CurrentLocation.Z = InitialLocation.Z + Amplitude * FMath::Sin(Frequency * Time);
	MeshScene->SetRelativeLocation(CurrentLocation);
}

