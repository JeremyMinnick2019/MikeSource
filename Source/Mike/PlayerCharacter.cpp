// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GroomComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacterAnimInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "LongGun.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArmLength = 200.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	Camera->SetupAttachment(SpringArm);

	Camera->bUsePawnControlRotation = false;

	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	Face->SetupAttachment(GetMesh());

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(Face);

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(Face);

	Fuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("Fuzz"));
	Fuzz->SetupAttachment(Face);

	Eyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyelashes"));
	Eyelashes->SetupAttachment(Face);

	Mustache = CreateDefaultSubobject<UGroomComponent>(TEXT("Mustache"));
	Mustache->SetupAttachment(Face);

	Beard = CreateDefaultSubobject<UGroomComponent>(TEXT("Beard"));
	Beard->SetupAttachment(Face);

	Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(GetMesh());

	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(GetMesh());

	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	Feet->SetupAttachment(GetMesh());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	bCanRun = false;

	RunningSpeed = 600;

	bIsTriggerpulled = false;

	bIsFirstRound = false;

	bIsADS = false;


}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	SpringArm->TargetArmLength = SpringArmLength;
	StartingWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInstance->bIsFalling = GetCharacterMovement()->IsFalling();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("CanRun"), EInputEvent::IE_Pressed, this, &APlayerCharacter::CanRun);
	PlayerInputComponent->BindAction(TEXT("CanRun"), EInputEvent::IE_Released, this, &APlayerCharacter::CanRun);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("PickUp"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PickUp);

	PlayerInputComponent->BindAction(TEXT("RightClick"), EInputEvent::IE_Pressed, this, &APlayerCharacter::RightClickPressed);
	PlayerInputComponent->BindAction(TEXT("RightClick"), EInputEvent::IE_Released, this, &APlayerCharacter::RightClickReleased);

	PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Released, this, &APlayerCharacter::ReleaseTrigger);

}

void APlayerCharacter::MoveForward(float Value)
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (bCanRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = StartingWalkingSpeed;
	}

	//Every character has a controller with holds the rotation
	const FRotator Rotation = Controller->GetControlRotation();

	//Pitch = y turn, Yaw = z turn, Roll = x turn 
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	//I got nothing for this I need to spend time learn Matrixs
	const  FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Value);

	AnimInstance->ForwardMovement = Value;
}

void APlayerCharacter::MoveRight(float Value)
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (bCanRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = StartingWalkingSpeed;
	}

	//Samething as move forward but GetUnitAxis Y

	//Every character has a controller with holds the rotation
	const FRotator Rotation = Controller->GetControlRotation();

	//Pitch = y turn, Yaw = z turn, Roll = x turn 
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	//I got nothing for this I need to spend time learn Matrixs
	const  FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Value);

	AnimInstance->SideMovement = Value;
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::CanRun()
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (bCanRun)
	{
		bCanRun = false;
		AnimInstance->bIsRunning = bCanRun;
	}
	else
	{
		bCanRun = true;
		AnimInstance->bIsRunning = bCanRun;
	}
	
}

void APlayerCharacter::PickUp()
{
	const USkeletalMeshSocket* RightHand = GetMesh()->GetSocketByName("Hand_RSocket");

	if (RightHand == nullptr || LongGun == nullptr)
	{
		return;
	}

	RightHand->AttachActor(LongGun, GetMesh());
	LongGun->SetOwner(this);

	AnimInstance->bHasLongGun = true;
}

void APlayerCharacter::RightClickPressed()
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	bIsADS = true;

	AnimInstance->SetIsADS(true);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
}

void APlayerCharacter::RightClickReleased()
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	bIsADS = false;

	AnimInstance->SetIsADS(false);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
}

void APlayerCharacter::PullTrigger()
{
	if (LongGun == nullptr)
	{
		return;
	}

	if (LongGun->CurrentAmmo == 0)
	{
		LongGun->GunEmpty();
		return;
	}

	bIsTriggerpulled = true;
	bIsFirstRound = true;

	HoldTrigger();
}

void APlayerCharacter::HoldTrigger()
{
	if (LongGun == nullptr)
	{
		return;
	}

	if (!bIsADS)
	{
		bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;
	}

	if (bIsFirstRound != true && bIsTriggerpulled == true && LongGun->CurrentAmmo != 0)
	{
		GetWorldTimerManager().SetTimer(PlayerFiresWeapon, this, &APlayerCharacter::GunFires, LongGun->RateOfFire);
	}
	else if (bIsFirstRound == true && bIsTriggerpulled == true && LongGun->CurrentAmmo != 0)
	{
		bIsFirstRound = false;
		GunFires();
	}
}

void APlayerCharacter::GunFires()
{
	if (LongGun == nullptr || AnimInstance == nullptr)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(LongGun->SmokeTimer);
	LongGun->PullTrigger();

	if (bIsADS)
	{
		AnimInstance->Montage_Play(PlayerCharacterAnimMontage, 0.75);
		AnimInstance->Montage_JumpToSection(TEXT("FireRifleIron"), PlayerCharacterAnimMontage);
	}
	else
	{
		AnimInstance->Montage_Play(PlayerCharacterAnimMontage, 0.75);
		AnimInstance->Montage_JumpToSection(TEXT("FireRifleHip"), PlayerCharacterAnimMontage);
	}

	AddControllerPitchInput(LongGun->Recoil);

	if (LongGun->bSemiAuto == false)
	{
		HoldTrigger();
	}
}

void APlayerCharacter::ReleaseTrigger()
{
	GetWorldTimerManager().ClearTimer(PlayerFiresWeapon);
	bIsFirstRound = false;
	bIsTriggerpulled = false;
	if (!bIsADS)
	{
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	SetActorRotation(FQuat(FRotator(0.f, Rotation.Yaw, 0.f)));
}

void APlayerCharacter::ChangeSelectFire()
{
}
