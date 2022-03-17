// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LongGun.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_Rifle UMETA(DisplayName = "Rifle"),
	EAT_Shotgun UMETA(DisplayName = "Shotgun"),
	EAT_Pistol UMETA(DisplayName = "Pistol"),

	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class MIKE_API ALongGun : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALongGun();

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY()
	class ANewPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* GunShotSound;

	UPROPERTY(EditDefaultsOnly)
    USoundBase* EmptyGunSound;

	UPROPERTY(EditDefaultsOnly)
	FVector RelativeLocationSettings;

	UPROPERTY(EditDefaultsOnly)
	FRotator RelativeRotationSettings;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SelectFireSound;

	UPROPERTY()
	bool bHasMultipleFiringModes;

	UPROPERTY()
	bool bSemiAuto;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* MuzzelFlash;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* MuzzelSmoke;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* ShellEject;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImpactEffect;

	UPROPERTY()
	FTimerHandle SmokeTimer;

	UPROPERTY(EditDefaultsOnly)
	EAmmoType AmmoType;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentAmmo;

	UFUNCTION()
	void SetCurrentAmmo(int32 Value) { CurrentAmmo = Value; }

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly)
	float RateOfFire;

	UPROPERTY(EditDefaultsOnly)
	float Recoil;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void PullTrigger();

	UFUNCTION()
	void GunSmoke();

	UFUNCTION()
	void GunEmpty();

	UFUNCTION()
	bool BulletFired(FHitResult& Hit, FVector& ShotDirection);

	UFUNCTION()
	bool DidChangeFiringMode();

	UFUNCTION()
	void DropGun();

	UFUNCTION()
	void EnableComponentsSimulatePhysicsCustom();

	UFUNCTION()
	void DisableComponentsSimulatePhysicsCustom();

	UFUNCTION()
	void RelativeLocationFix();

	UFUNCTION()
	AController* GetOwnerController() const;
};
