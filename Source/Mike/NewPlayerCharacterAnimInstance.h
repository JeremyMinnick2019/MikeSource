// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NewPlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIKE_API UNewPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		float ForwardMovement;

	UPROPERTY(BlueprintReadOnly)
		float SideMovement;

	UPROPERTY(BlueprintReadOnly)
		bool bIsRunning;

	UPROPERTY(BlueprintReadOnly)
		bool bIsFalling;

	UPROPERTY(BlueprintReadOnly)
		bool bHasLongGun;

	UPROPERTY(BlueprintReadOnly)
	bool bIsADS;

	UPROPERTY(BlueprintReadOnly)
	bool bCanRun;

	UPROPERTY(BlueprintReadOnly)
	bool IsDead;

	UFUNCTION()
	void SetIsADS(bool Value) { bIsADS = Value; }
	
};
