// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"


// - - - - - - - - - - - - - - - - - - - - - -
//			PUBLIC FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - -

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health Left: %f"), Health);

	return DamageToApply;
}

void AShooterCharacter::Shoot()
{
	Gun->PulLTrigger();
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

// - - - - - - - - - - - - - - - - - - - - - -
//			PROTECTED FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - -

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}


// - - - - - - - - - - - - - - - - - - - - - -
//			PRIVATE FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - -

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

/*
	Calculating Animation Speed - Watching foot in Animation - T1 and Y1 are the Time and Y value when the 
	foot touches the ground the first time. T2 and Y2 are the Time and Y value when the foot leaves the ground
	after the first time.
	Following equation:
	(Y2 - Y1) / (T2 - T1) = Animation Speed

	Data for Forward Jog Animation
	T1: .16
	Y1: 17.088177
	T2: .30
	Y2: -28.421715
	Y2 - Y1: 
	T2 - T1: .14
	-45.509892 / .14 = -325.0706571428571
	Rounded: -350

	Result for Forward Walk Animation
	(-22.001398 - 20.976902) / (.67 - .34) = -130.23727272727
	Rounded: -150
*/
