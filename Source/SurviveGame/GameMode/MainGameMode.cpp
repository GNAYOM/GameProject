// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

#include "SurviveGame/PlayerState/MainPlayerState.h"

AMainGameMode::AMainGameMode()
{
	GameStateClass = AMainGameState::StaticClass();
	PlayerStateClass = AMainPlayerState::StaticClass();
}
