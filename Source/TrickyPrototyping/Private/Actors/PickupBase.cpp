// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/PickupBase.h"
#include "Components/TriggerComponents/InteractionSphereComponent.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionTrigger = CreateDefaultSubobject<UInteractionSphereComponent>("InteractionTrigger");
	SetRootComponent(InteractionTrigger);

	MeshScene = CreateDefaultSubobject<USceneComponent>("MeshScene");
	MeshScene->SetupAttachment(GetRootComponent());
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = MeshScene->GetRelativeLocation();
	InteractionTrigger->SetIsNormalTrigger(bRequireInteraction);

	if (bRequireInteraction)
	{
		InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnTriggerBeginOverlap);
	}
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimatePosition();
	AnimateRotation();
}

bool APickupBase::ActivatePickup_Implementation(APawn* TargetPawn)
{
	return false;
}

bool APickupBase::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController || !PlayerController->GetPawn() || !bRequireInteraction) return false;

	return ActivatePickup(PlayerController->GetPawn());
}

void APickupBase::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
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

void APickupBase::AnimateRotation() const
{
	if (!bAnimateRotation) return;

	MeshScene->AddRelativeRotation(FRotator(0.f, RotationSpeed, 0.f));
}

void APickupBase::AnimatePosition() const
{
	if (!bAnimatePosition || !GetWorld()) return;

	const float Time = GetWorld()->GetTimeSeconds();
	FVector CurrentLocation = MeshScene->GetRelativeLocation();
	CurrentLocation.Z = InitialLocation.Z + Amplitude * FMath::Sin(Frequency * Time);
	MeshScene->SetRelativeLocation(CurrentLocation);
}