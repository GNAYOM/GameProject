// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGameStateInterface.h"
#include "GameFramework/GameStateBase.h"
#include "SurviveGame/InteractObject/ActorInteractObjectInterface.h"
#include "InteractObjectSystem.h"
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
	virtual InteractObjectSystem* CreateInteractObjectSystem(AActorInteractObjectInterface*) override;
	TArray<InteractObjectSystem* > InteractObjectSystems;
};
