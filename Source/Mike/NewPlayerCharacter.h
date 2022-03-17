// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NewPlayerCharacter.generated.h"

UCLASS()
class MIKE_API ANewPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANewPlayerCharacter();

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmLength;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmLengthInit;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmLengthADS;

	UPROPERTY(EditDefaultsOnly)
	float ADSSpeed;

	UPROPERTY()
	class UNewPlayerCharacterAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* Head;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* Legs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* Hands;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* EyeBrow;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Eyes;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Hair;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Beard;

	UPROPERTY()
	bool bShiftIsDown;

	UPROPERTY(EditDefaultsOnly)
	float Health;

	UPROPERTY()
	bool bCanRun;

	UPROPERTY()
	bool bIsOverLapping;

	UPROPERTY()
	bool bInitPickup;

	UPROPERTY()
	float StartingWalkingSpeed;

	UPROPERTY(EditDefaultsOnly)
	float RunningSpeed;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentAmmoInMag;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxRifleAmmoReserve;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentRifleAmmoReserve;

	UPROPERTY()
	class ALongGun* LongGunOverlapped;

	UPROPERTY()
	class ALongGun* LongGunCurrent;

	UPROPERTY()
	class ALongGun* LongGunOld;

	UPROPERTY()
	bool bIsTriggerpulled;

	UPROPERTY()
	bool bIsFirstRound;

	UPROPERTY()
	bool bIsADS;

	UPROPERTY()
	bool IsDead;

	UPROPERTY()
	FTimerHandle PlayerFiresWeapon;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* PlayerCharacterAnimMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void LookUp(float Value);

	UFUNCTION()
	void LookRight(float Value);

	UFUNCTION()
	void CanRun();

	UFUNCTION()
	void ShiftDown();

	UFUNCTION()
	void ShiftUp();

	void SetActiveOverlappingItem(ALongGun* LongGunPass);

	UFUNCTION()
	void PickUp();

	UFUNCTION()
	void RightClickPressed();

	UFUNCTION()
	void RightClickReleased();

	UFUNCTION()
	void PullTrigger();

	UFUNCTION()
	void HoldTrigger();

	UFUNCTION()
	void GunFires();

	UFUNCTION()
	void ReleaseTrigger();

	UFUNCTION()
	void ChangeSelectFire();

	UFUNCTION()
	void AimDownSight();

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void TakingDamage(float Damage);

	UFUNCTION()
	void Death();

	UFUNCTION()
	void SetCollisionToNone();
};
