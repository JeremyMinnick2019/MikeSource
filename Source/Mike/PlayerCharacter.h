// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArm;

	// So I can access in blueprints and code
	UPROPERTY(EditDefaultsOnly)
	float SpringArmLength;

	UPROPERTY()
	class UPlayerCharacterAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* Face;

	UPROPERTY(EditDefaultsOnly)
	class UGroomComponent* Hair;

	UPROPERTY(EditDefaultsOnly)
	UGroomComponent* Eyebrows;

	UPROPERTY(EditDefaultsOnly)
	UGroomComponent* Fuzz;

	UPROPERTY(EditDefaultsOnly)
	UGroomComponent* Eyelashes;

	UPROPERTY(EditDefaultsOnly)
	UGroomComponent* Mustache;

	UPROPERTY(EditDefaultsOnly)
	UGroomComponent* Beard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMeshComponent* Torso;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMeshComponent* Legs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMeshComponent* Feet;

	UPROPERTY()
	bool bCanRun;

	UPROPERTY()
	float StartingWalkingSpeed;

	UPROPERTY(EditDefaultsOnly)
	float RunningSpeed;

	UPROPERTY()
	class ALongGun* LongGun;

	UPROPERTY()
	bool bIsTriggerpulled;

	UPROPERTY()
	bool bIsFirstRound;

	UPROPERTY()
	bool bIsADS;

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

	FORCEINLINE void SetActiveOverlappingItem(ALongGun* LongGunPass) { LongGun = LongGunPass; }

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

};
