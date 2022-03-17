// Fill out your copyright notice in the Description page of Project Settings.


#include "NewPlayerCharacter.h"
#include "GroomComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NewPlayerCharacterAnimInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "LongGun.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/Class.h"


// Sets default values
ANewPlayerCharacter::ANewPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArmLength = 200.f;
	SpringArmLengthADS = 75.f;
    ADSSpeed = 1.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	Camera->SetupAttachment(SpringArm);

	Camera->bUsePawnControlRotation = false;

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetMesh());

	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(GetMesh());
	
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Hands->SetupAttachment(GetMesh());
	
	EyeBrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeBrow"));
	EyeBrow->AttachToComponent(Head, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
	//EyeBrow->SetupAttachment(Head);
	
	Eyes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	Eyes->AttachToComponent(Head, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
	//Eyes->SetupAttachment(Head);
	
	Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	Hair->AttachToComponent(Head, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
	//Hair->SetupAttachment(Head);
	
	Beard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beard"));
	Beard->AttachToComponent(Head, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
	//Beard->SetupAttachment(Head);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	bCanRun = true;

	bShiftIsDown = false;

	RunningSpeed = 600;

	bIsTriggerpulled = false;

	bIsFirstRound = false;

	bIsADS = false;

	bIsOverLapping = false;

	bInitPickup = true;

	Health = 100.f;

	IsDead = false;
}

// Called when the game starts or when spawned
void ANewPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UNewPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->bCanRun = true;
	AnimInstance->IsDead = false;
	SpringArm->TargetArmLength = SpringArmLength;
	StartingWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentRifleAmmoReserve = MaxRifleAmmoReserve;
	SpringArmLengthInit = SpringArmLength;
}

// Called every frame
void ANewPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInstance->bIsFalling = GetCharacterMovement()->IsFalling();

	AimDownSight();

}

// Called to bind functionality to input
void ANewPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANewPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANewPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ANewPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ANewPlayerCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("CanRun"), EInputEvent::IE_Pressed, this, &ANewPlayerCharacter::ShiftDown);
	PlayerInputComponent->BindAction(TEXT("CanRun"), EInputEvent::IE_Released, this, &ANewPlayerCharacter::ShiftUp);

	//PlayerInputComponent->BindAction(TEXT("CanRun"), EInputEvent::IE_Pressed, this, &ANewPlayerCharacter::CanRun);
	//PlayerInputComponent->BindAction(TEXT("CanRun"), EInputEvent::IE_Released, this, &ANewPlayerCharacter::CanRun);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("PickUp"), EInputEvent::IE_Pressed, this, &ANewPlayerCharacter::PickUp);

	PlayerInputComponent->BindAction(TEXT("RightClick"), EInputEvent::IE_Pressed, this, &ANewPlayerCharacter::RightClickPressed);
	PlayerInputComponent->BindAction(TEXT("RightClick"), EInputEvent::IE_Released, this, &ANewPlayerCharacter::RightClickReleased);

	PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Pressed, this, &ANewPlayerCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Released, this, &ANewPlayerCharacter::ReleaseTrigger);

	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ANewPlayerCharacter::Reload);

}

void ANewPlayerCharacter::MoveForward(float Value)
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (bCanRun && bShiftIsDown)
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

void ANewPlayerCharacter::MoveRight(float Value)
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (bCanRun && bShiftIsDown)
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

void ANewPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ANewPlayerCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value);
}

void ANewPlayerCharacter::CanRun()
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

void ANewPlayerCharacter::ShiftDown()
{
	bShiftIsDown = true;

	if (bCanRun)
	{
		AnimInstance->bIsRunning = true;
	}
}

void ANewPlayerCharacter::ShiftUp()
{
	bShiftIsDown = false;

	AnimInstance->bIsRunning = false;
}

void ANewPlayerCharacter::SetActiveOverlappingItem(ALongGun* LongGunPass)
{
	if (LongGunPass == nullptr)
	{
		return;
	}

	LongGunOverlapped = LongGunPass;
}

void ANewPlayerCharacter::PickUp()
{

	if (!bIsOverLapping)
	{
		return;
	}

	const USkeletalMeshSocket* RightHand = GetMesh()->GetSocketByName("hand_rSocket");

	LongGunOld = LongGunCurrent;

	LongGunCurrent = LongGunOverlapped;

	if (RightHand == nullptr || LongGunCurrent == nullptr)
	{
		return;
	}

	LongGunCurrent->DisableComponentsSimulatePhysicsCustom();

	if (LongGunOld != nullptr)
	{
		//TSubclassOf<ALongGun> LongGunSpawn = LongGunOld->GetClass();

		//GetWorld()->SpawnActor<ALongGun>(LongGunSpawn, LongGunCurrent->GetActorLocation(), LongGunCurrent->GetActorRotation());

		LongGunOld->DropGun();
	}
	

	RightHand->AttachActor(LongGunCurrent, GetMesh());
	LongGunCurrent->SetOwner(this);
	LongGunCurrent->RelativeLocationFix();

	AnimInstance->bHasLongGun = true;
	CurrentAmmoInMag = LongGunCurrent->CurrentAmmo;

	if (bInitPickup)
	{
		bIsOverLapping = false;
		bInitPickup = false;
	}
}

void ANewPlayerCharacter::RightClickPressed()
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	bIsADS = true;
	bCanRun = false;
	AnimInstance->bCanRun = bCanRun;

	AnimInstance->SetIsADS(true);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
}

void ANewPlayerCharacter::RightClickReleased()
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	bIsADS = false;
	bCanRun = true;
	AnimInstance->bCanRun = bCanRun;

	if (bShiftIsDown)
	{
		AnimInstance->bIsRunning = true;
	}

	

	AnimInstance->SetIsADS(false);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	const FRotator Rotation = Controller->GetControlRotation();
	SetActorRotation(FQuat(FRotator(0.f, Rotation.Yaw, 0.f)));
}

void ANewPlayerCharacter::PullTrigger()
{
	if (LongGunCurrent == nullptr)
	{
		return;
	}

	if (LongGunCurrent->CurrentAmmo == 0)
	{
		LongGunCurrent->GunEmpty();
		return;
	}

	bIsTriggerpulled = true;
	bIsFirstRound = true;

	HoldTrigger();
}

void ANewPlayerCharacter::HoldTrigger()
{
	if (LongGunCurrent == nullptr)
	{
		return;
	}

	if (!bIsADS)
	{
		bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;
	}

	if (bIsFirstRound != true && bIsTriggerpulled == true && LongGunCurrent->CurrentAmmo != 0)
	{
		GetWorldTimerManager().SetTimer(PlayerFiresWeapon, this, &ANewPlayerCharacter::GunFires, LongGunCurrent->RateOfFire);
	}
	else if (bIsFirstRound == true && bIsTriggerpulled == true && LongGunCurrent->CurrentAmmo != 0)
	{
		bIsFirstRound = false;
		GunFires();
	}
}

void ANewPlayerCharacter::GunFires()
{
	if (LongGunCurrent == nullptr || AnimInstance == nullptr)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(LongGunCurrent->SmokeTimer);
	LongGunCurrent->PullTrigger();
	CurrentAmmoInMag = LongGunCurrent->CurrentAmmo;

	//looks bad
	/*if (bIsADS)
	{
		AnimInstance->Montage_Play(PlayerCharacterAnimMontage, 0.75);
		AnimInstance->Montage_JumpToSection(TEXT("FireRifleIron"), PlayerCharacterAnimMontage);
	}
	else
	{
		AnimInstance->Montage_Play(PlayerCharacterAnimMontage, 0.75);
		AnimInstance->Montage_JumpToSection(TEXT("FireRifleHip"), PlayerCharacterAnimMontage);
	}*/

	AddControllerPitchInput(LongGunCurrent->Recoil);

	if (LongGunCurrent->bSemiAuto == false)
	{
		HoldTrigger();
	}
}

void ANewPlayerCharacter::ReleaseTrigger()
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

void ANewPlayerCharacter::ChangeSelectFire()
{
}

void ANewPlayerCharacter::AimDownSight()
{
	if (bIsADS == true)
	{
		if (SpringArmLength != SpringArmLengthADS)
		{
			SpringArmLength = SpringArmLength - ADSSpeed * GetWorld()->GetDeltaSeconds();
			if (SpringArmLength < SpringArmLengthADS)
			{
				SpringArmLength = SpringArmLengthADS;
			}
		}
		SpringArm->TargetArmLength = SpringArmLength;
	}
	else
	{
		if (SpringArmLength != SpringArmLengthInit)
		{
			SpringArmLength = SpringArmLength + ADSSpeed * GetWorld()->GetDeltaSeconds();
			if (SpringArmLength > SpringArmLengthInit)
			{
				SpringArmLength = SpringArmLengthInit;
			}
		}
		SpringArm->TargetArmLength = SpringArmLength;
	}
}

void ANewPlayerCharacter::Reload()
{
	if (LongGunCurrent == nullptr || AnimInstance == nullptr)
	{
		return;
	}

	if (LongGunCurrent->CurrentAmmo == LongGunCurrent->MaxAmmo)
	{
		return;
	}

	switch (LongGunCurrent->AmmoType)
	{
	case EAmmoType::EAT_Rifle:

		if (CurrentRifleAmmoReserve == 0)
		{
			return;
		}

		if (CurrentRifleAmmoReserve <= LongGunCurrent->MaxAmmo)
		{
			AnimInstance->Montage_Play(PlayerCharacterAnimMontage);
			AnimInstance->Montage_JumpToSection(TEXT("Reload"), PlayerCharacterAnimMontage);

			if (LongGunCurrent->CurrentAmmo != 0)
			{
				if (LongGunCurrent->CurrentAmmo + CurrentRifleAmmoReserve > LongGunCurrent->MaxAmmo)
				{
					int32 AmmoSpent = LongGunCurrent->MaxAmmo - LongGunCurrent->CurrentAmmo;
					CurrentRifleAmmoReserve = CurrentRifleAmmoReserve - AmmoSpent;
					LongGunCurrent->SetCurrentAmmo(LongGunCurrent->MaxAmmo);
					CurrentAmmoInMag = LongGunCurrent->CurrentAmmo;
				}
				else
				{
					LongGunCurrent->SetCurrentAmmo(LongGunCurrent->CurrentAmmo + CurrentRifleAmmoReserve);
					CurrentRifleAmmoReserve = 0;
					CurrentAmmoInMag = LongGunCurrent->CurrentAmmo;
				}
			}
			else
			{
				LongGunCurrent->SetCurrentAmmo(CurrentRifleAmmoReserve);
				CurrentAmmoInMag = LongGunCurrent->CurrentAmmo;
				CurrentRifleAmmoReserve = 0;
			}
		}
		else
		{
			AnimInstance->Montage_Play(PlayerCharacterAnimMontage);
			AnimInstance->Montage_JumpToSection(TEXT("Reload"), PlayerCharacterAnimMontage);
			if (LongGunCurrent->CurrentAmmo != 0)
			{
				int32 AmmoSpent = LongGunCurrent->MaxAmmo - LongGunCurrent->CurrentAmmo;
				CurrentRifleAmmoReserve = CurrentRifleAmmoReserve - AmmoSpent;
				LongGunCurrent->SetCurrentAmmo(LongGunCurrent->MaxAmmo);
				CurrentAmmoInMag = LongGunCurrent->CurrentAmmo;
			}
			else
			{
				CurrentRifleAmmoReserve = CurrentRifleAmmoReserve - LongGunCurrent->MaxAmmo;
				LongGunCurrent->SetCurrentAmmo(LongGunCurrent->MaxAmmo);
				CurrentAmmoInMag = LongGunCurrent->CurrentAmmo;
			}
		}

		break;

	case EAmmoType::EAT_Shotgun:

		break;

	case EAmmoType::EAT_Pistol:

		break;

	case EAmmoType::EAT_MAX:
		break;

	default:
		break;
	}

}

void ANewPlayerCharacter::TakingDamage(float Damage)
{
	Health = Health - Damage;

	UE_LOG(LogTemp, Warning , TEXT("%f"), Health)

	if (Health <= 0 && IsDead != true)
	{
		IsDead = true;
		Death();
	}
}

void ANewPlayerCharacter::Death()
{
	GetController()->Destroy();

	SetCollisionToNone();

	AnimInstance->IsDead = true;
	
}

void ANewPlayerCharacter::SetCollisionToNone()
{
	UActorComponent* Component = GetRootComponent();

	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Component))
	{
		PrimComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
}

