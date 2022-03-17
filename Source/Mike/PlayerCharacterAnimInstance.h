// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIKE_API UPlayerCharacterAnimInstance : public UAnimInstance
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

	UFUNCTION()
	void SetIsADS(bool Value) { bIsADS = Value; }
};
