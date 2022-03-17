// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NewPlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class MIKE_API ANewPlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* HUDOverlay;

protected:
	
	virtual void BeginPlay() override;

};
