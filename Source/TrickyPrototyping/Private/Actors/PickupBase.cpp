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
	Super::BeginPlay();

	InitialLocation = MeshScene->GetRelativeLocation();
	InteractionTrigger->SetIsNormalTrigger(!bRequireInteraction);
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnTriggerBeginOverlap);
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimatePosition();
	AnimateRotation();
}

void APickupBase::ActivatePickup_Implementation(AActor* TargetActor)
{

}

void APickupBase::DestroyPickup()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	InteractionTrigger->SetIsEnabled(false);
	Destroy();
}

bool APickupBase::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController || !PlayerController->GetPawn() || !bRequireInteraction) return false;

	ActivatePickup(Cast<AActor>(PlayerController->GetPawn()));
	DestroyPickup();
	
	return true;
}

void APickupBase::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex,
                                        bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (bRequireInteraction || !IsValid(OtherActor)) return;

	ActivatePickup(OtherActor);
	DestroyPickup();
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
