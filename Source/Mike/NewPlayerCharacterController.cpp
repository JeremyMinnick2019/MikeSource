// Fill out your copyright notice in the Description page of Project Settings.


#include "NewPlayerCharacterController.h"
#include "Blueprint/UserWidget.h"

void ANewPlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}
