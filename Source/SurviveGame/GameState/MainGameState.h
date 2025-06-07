// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVEGAME_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()
	AMainGameState();
	~AMainGameState() = default;
public:
	//PlayerStatus
	FString WeightStatus;
	float WeightLoad = 50;
};
