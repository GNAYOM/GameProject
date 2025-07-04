// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "SurviveGame/InteractObject/InteractObjectProperty.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVEGAME_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	AMainPlayerState();
	~AMainPlayerState() = default;
public:
	//PlayerStatus
	FString WeightStatus;
	float WeightLoad = 50;
	FVector CurrentDirectionNormal;
	FVector BackSocketCurrentLocation;
	UPROPERTY()
	UStaticMeshComponent* BackSocket;
};
