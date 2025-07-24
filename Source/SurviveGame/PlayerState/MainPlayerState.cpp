// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"

AMainPlayerState::AMainPlayerState()
{
	WeightLoad = 100000.0f;
	PossessedSystem = new InteractObjectSystem();
	BackSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackSocket"));
	InteractManagerComponentStatus = SelectByRange;
}

void AMainPlayerState::PrintSocketState()
{
	FString Tmp_PrintRow;
	FString Tmp_CurrChar;
	if(PossessedSocketPanel!=NULL)
	for (int i = 0;i < PossessedSocketPanel->Panel.Num();i++)
	{
		Tmp_PrintRow = "";
		for(int j = 0;j< PossessedSocketPanel->PanelRow.Num();j++)
		{
			Tmp_CurrChar = FString::FromInt(PossessedSocketPanel->Panel[i][j].IsOccupied);
			Tmp_PrintRow += Tmp_CurrChar + " ";
		}
		UE_LOG(LogTemp,Warning,TEXT("%s"),*Tmp_PrintRow)
	}
}
