// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoCrate.h"
#include "Components/SphereComponent.h"
#include "NewPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AAmmoCrate::AAmmoCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereComponent);

	FillTime = 0.1f;
}

// Called when the game starts or when spawned
void AAmmoCrate::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoCrate::CombatOnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAmmoCrate::CombatOnOverlapEnd);
	
}

// Called every frame
void AAmmoCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoCrate::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<ANewPlayerCharacter>(OtherActor);

	if (PlayerCharacter == nullptr || PlayerCharacter->CurrentRifleAmmoReserve == PlayerCharacter->MaxRifleAmmoReserve)
	{
		return;
	}

	FillAmmo();
}

void AAmmoCrate::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(AmmoFill);
}

void AAmmoCrate::FillAmmo()
{
	PlayerCharacter->CurrentRifleAmmoReserve += 1;

	GetWorldTimerManager().SetTimer(AmmoFill, this, &AAmmoCrate::FillAmmo, FillTime);

	if (PlayerCharacter->CurrentRifleAmmoReserve == PlayerCharacter->MaxRifleAmmoReserve)
	{
		GetWorldTimerManager().ClearTimer(AmmoFill);
	}
}

