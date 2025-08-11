// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "SurviveGame/InteractObject/InteractObjectProperty.h"
#include "SurviveGame/GameState/InteractObjectSystem.h"
#include "SurviveGame/Enum/InteractManagerComponentStatusEnum/InteractManagerComponentStatusEnum.h"
#include "SurviveGame/InteractManagerComponent/InteractManagerComponentInterface.h"
#include "SurviveGame/InteractObject/InteractObjectSocket.h"
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
	InteractObjectSystem* PossessedSystem;
	SocketPanel* PossessedSocketPanel;
	UPROPERTY()
	UStaticMeshComponent* BackSocket;
	UPROPERTY()
	UInteractManagerComponentInterface* PlayerPossessedInteractManagerComponent;
	void PrintSocketState();
};
