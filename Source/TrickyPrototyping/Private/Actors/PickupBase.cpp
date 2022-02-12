// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/PickupBase.h"
#include "Components/TriggerComponents/InteractionSphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupRoot = CreateDefaultSubobject<USceneComponent>("PickupRoot");
	SetRootComponent(PickupRoot);

	InteractionTrigger = CreateDefaultSubobject<UInteractionSphereComponent>("InteractionTrigger");
	InteractionTrigger->SetupAttachment(GetRootComponent());

	MeshScene = CreateDefaultSubobject<USceneComponent>("MeshScene");
	MeshScene->SetupAttachment(GetRootComponent());
}

void APickupBase::BeginPlay()
{
	InitialLocation = MeshScene->GetRelativeLocation();
	InteractionTrigger->SetIsNormalTrigger(!bRequireInteraction);
	InteractionTrigger->bRequireLineOfSight = bRequireLineOfSight;
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnTriggerBeginOverlap);
	
	Super::BeginPlay();
}

void APickupBase::Tick(float DeltaTime)
{
	AnimatePosition();
	AnimateRotation();
	
	Super::Tick(DeltaTime);
}

void APickupBase::ActivatePickup_Implementation()
{
	if (!GetRootComponent()->bHiddenInGame) return;

	GetRootComponent()->SetHiddenInGame(false, true);
	PrimaryActorTick.bCanEverTick = true;
	InteractionTrigger->SetIsEnabled(true);
}

bool APickupBase::ActivatePickupEffect_Implementation(AActor* TargetActor)
{
	return true;
}

void APickupBase::DestroyPickup()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	InteractionTrigger->SetIsEnabled(false);
	Destroy();
}

void APickupBase::DeactivatePickup_Implementation()
{
	GetRootComponent()->SetHiddenInGame(true, true);
	PrimaryActorTick.bCanEverTick = false;
	InteractionTrigger->SetIsEnabled(false);
}

bool APickupBase::ProcessInteraction_Implementation(AActor* TargetActor)
{
	if (!TargetActor || !bRequireInteraction) return false;

	if (ActivatePickupEffect(TargetActor))
	{
		bDestroyOnEffectActivation ? DestroyPickup() : DeactivatePickup();
		return true;
	}

	return false;
}

void APickupBase::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex,
                                        bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (bRequireInteraction || !IsValid(OtherActor)) return;

	if (ActivatePickupEffect(OtherActor))
	{
		DestroyPickup();
	}
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
