// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// - - - - - - - - - - - - - - - - - - - - - -
//			PUBLIC FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - -

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PulLTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;

	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;


	FHitResult Hit;

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		AActor* HitActor = Hit.GetActor();

		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - -
//			PROTECTED FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - -

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}


