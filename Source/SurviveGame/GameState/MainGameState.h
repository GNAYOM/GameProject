// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGameStateInterface.h"
#include "GameFramework/GameStateBase.h"
#include "SurviveGame/InteractObject/ActorInteractObjectInterface.h"
#include "InteractObjectSystem.h"
#include "SurviveGame/PlayerState/MainPlayerState.h"
#include "MainGameState.generated.h"


/**
 * 
 */

UCLASS()
class SURVIVEGAME_API AMainGameState : public AMainGameStateInterface
{
	GENERATED_BODY()
	AMainGameState();
	~AMainGameState() = default;
	//InteractObjectSystem
public:
	UPROPERTY()
	AMainPlayerState* MainPlayerState;
	virtual void BeginPlay() override;
	virtual InteractObjectSystem* CreateInteractObjectSystem(AActorInteractObjectInterface* FirstInteractObject) override;
	virtual void InteractObjectSystemAddInteractObject(InteractObjectSystem* TargetInteractObjectSystem, AActorInteractObjectInterface* NewInteractObject) override;
	virtual InteractObjectSystem* MergeInteractObjectSystem(InteractObjectSystem* Source1,InteractObjectSystem* Source2,
			int ConnectObject1Index,int ConnectObject2Index) override;
	virtual void PrintConnectionMatrix(InteractObjectSystem PrintTarget) override;
	TArray<InteractObjectSystem* > InteractObjectSystems;
};
