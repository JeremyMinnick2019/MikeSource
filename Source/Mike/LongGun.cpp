// Fill out your copyright notice in the Description page of Project Settings.


#include "LongGun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NewPlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
ALongGun::ALongGun()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(Mesh);
	//SetRootComponent(GetRootComponent());

	RateOfFire = 0.0f;

	bHasMultipleFiringModes = false;

	bSemiAuto = false;

	Recoil = 0.25f;

	MaxAmmo = 30;
}

//float ALongGun::Damage(FHitResult& Hit)
//{
//	PlayerCharacter = Cast<ANewPlayerCharacter>(Hit.GetActor());
//
//
//	return 0.0f;
//}

// Called when the game starts or when spawned
void ALongGun::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ALongGun::CombatOnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ALongGun::CombatOnOverlapEnd);
	
}

//This needs to be here even if not in use, will cause errors if deleted
// Called every frame
void ALongGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//This needs to be here even if not in use, will cause errors if deleted
void ALongGun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALongGun::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<ANewPlayerCharacter>(OtherActor);

	if (PlayerCharacter == nullptr)
	{
		return;
	}

	PlayerCharacter->SetActiveOverlappingItem(this);
	PlayerCharacter->bIsOverLapping = true;
}

void ALongGun::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerCharacter = Cast<ANewPlayerCharacter>(OtherActor);

	if (PlayerCharacter == nullptr)
	{
		return;
	}

	PlayerCharacter->SetActiveOverlappingItem(nullptr);
	PlayerCharacter->bIsOverLapping = false;
}

void ALongGun::PullTrigger()
{
	//b_gun_shellejectSocket
	CurrentAmmo = CurrentAmmo - 1;
	UNiagaraFunctionLibrary::SpawnSystemAttached(ShellEject, Mesh, TEXT("b_gun_shellejectSocket"), Mesh->GetSocketLocation(TEXT("b_gun_shellejectSocket")), Mesh->GetSocketRotation(TEXT("b_gun_shellejectSocket")), FVector(1.f), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true, true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzelFlash, Mesh->GetSocketLocation(TEXT("Muzzel")));
	UGameplayStatics::SpawnSoundAttached(GunShotSound, Mesh, TEXT("Muzzle"));
	GetWorldTimerManager().SetTimer(SmokeTimer, this, &ALongGun::GunSmoke, 0.4f);

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = BulletFired(Hit, ShotDirection);

	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
	}

	if (Cast<ANewPlayerCharacter>(Hit.GetActor()))
	{
		PlayerCharacter = Cast<ANewPlayerCharacter>(Hit.GetActor());

		PlayerCharacter->TakingDamage(10.f);
	}
	
}

bool ALongGun::BulletFired(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * 10000.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

}

void ALongGun::GunSmoke()
{
	UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzelSmoke, Mesh, TEXT("Muzzle"), Mesh->GetSocketLocation(TEXT("Muzzle")), FRotator(0.f), FVector(1.f), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true, true);
}

void ALongGun::GunEmpty()
{
	UGameplayStatics::SpawnSoundAttached(EmptyGunSound, Mesh, TEXT("b_gun_shellejectSocket"));
}

void ALongGun::DropGun()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	EnableComponentsSimulatePhysicsCustom();

	Mesh->AddForce(FVector(0.f, 0.f, 150000.f));
}

bool ALongGun::DidChangeFiringMode()
{
	if (bHasMultipleFiringModes)
	{
		if (bSemiAuto == false)
		{
			bSemiAuto = true;
		}
		else
		{
			bSemiAuto = false;
		}

		return true;
	}

	return false;
}

AController* ALongGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	return OwnerPawn->GetController();
}

void ALongGun::EnableComponentsSimulatePhysicsCustom()
{
	UActorComponent* Component = this->Mesh;

	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Component))
	{
		PrimComp->SetSimulatePhysics(true);
	}
}

void ALongGun::DisableComponentsSimulatePhysicsCustom()
{
	UActorComponent* Component = this->Mesh;

	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Component))
	{
		PrimComp->SetSimulatePhysics(false);
	}
}

void ALongGun::RelativeLocationFix()
{
	//(X=-8.702585,Y=-1.096784,Z=-9.373484)
	//(Pitch=-3.710413,Yaw=3.914874,Roll=-1.074492)
	SetActorRelativeLocation(RelativeLocationSettings);
	SetActorRelativeRotation(RelativeRotationSettings);
}

