// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"


AMainGameState::AMainGameState()
{
	
}

InteractObjectSystem* AMainGameState::CreateInteractObjectSystem(AActorInteractObjectInterface* FirstInteractObject)
{
	InteractObjectSystem* NewInteractObjectSystem = new InteractObjectSystem();
	
	NewInteractObjectSystem->TotalInteractObjects.Add(FirstInteractObject);
	TArray<int> ConnectionMatrixNewRow;
	NewInteractObjectSystem->ConnectionMatrix.Add(ConnectionMatrixNewRow);
	
	for (TArray<int> &CheckRow : NewInteractObjectSystem->ConnectionMatrix)
		while(CheckRow.Num() < NewInteractObjectSystem->TotalInteractObjects.Num())
			CheckRow.Add(0);
	
	InteractObjectSystems.Add(NewInteractObjectSystem);
	return NewInteractObjectSystem;
}
