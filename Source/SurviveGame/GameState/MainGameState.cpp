// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"

#include "Kismet/GameplayStatics.h"
#include "SurviveGame/PlayerState/MainPlayerState.h"


AMainGameState::AMainGameState()
{
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
	MainPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMainPlayerState>();
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

void AMainGameState::InteractObjectSystemAddInteractObject(InteractObjectSystem* TargetInteractObjectSystem,
	AActorInteractObjectInterface* NewInteractObject)
{
	TargetInteractObjectSystem->TotalInteractObjects.Add(NewInteractObject);
	TArray<int> ConnectionMatrixNewRow;
	TargetInteractObjectSystem->ConnectionMatrix.Add(ConnectionMatrixNewRow);
	
	for (TArray<int> &CheckRow : TargetInteractObjectSystem->ConnectionMatrix)
		while(CheckRow.Num() < TargetInteractObjectSystem->TotalInteractObjects.Num())
			CheckRow.Add(0);
}

InteractObjectSystem* AMainGameState::MergeInteractObjectSystem(InteractObjectSystem* Source1,InteractObjectSystem* Source2,
			int ConnectObject1Index,int ConnectObject2Index)
{
	int Source1Total = Source1->TotalInteractObjects.Num();
	int Source2Total = Source2->TotalInteractObjects.Num();
	
	InteractObjectSystem* NewInteractObjectSystem = new InteractObjectSystem();
	//两系统拥有的新系统指针指向新系统
	Source1->NewInteractObjectSystem = NewInteractObjectSystem;
	Source2->NewInteractObjectSystem = NewInteractObjectSystem;
	//IOI全部添加至新系统
	for(AActorInteractObjectInterface* I : Source1->TotalInteractObjects)
		NewInteractObjectSystem->TotalInteractObjects.Add(I);
	for(AActorInteractObjectInterface* I : Source2->TotalInteractObjects)
		NewInteractObjectSystem->TotalInteractObjects.Add(I);
	//设置行格式
	TArray<int> ConnectionMatrixNewRow;
	for (int i = 0;i < NewInteractObjectSystem->TotalInteractObjects.Num();i++)
		ConnectionMatrixNewRow.Add(0);
	//初始化连接矩阵
	for (int i = 0;i < NewInteractObjectSystem->TotalInteractObjects.Num();i++)
		NewInteractObjectSystem->ConnectionMatrix.Add(ConnectionMatrixNewRow);
	//复制连接矩阵
	for (int i = 0;i < Source1Total;i++)
		for(int j = 0;j<Source1Total;j++)
			NewInteractObjectSystem->ConnectionMatrix[i][j] = Source1->ConnectionMatrix[i][j];
	for (int i = Source1Total;i <Source1Total + Source2Total;i++)
		for(int j = Source1Total;j<Source1Total + Source2Total;j++)
			NewInteractObjectSystem->ConnectionMatrix[i][j] = Source1->ConnectionMatrix[i-Source1Total][j-Source1Total];
	//连接端点
	NewInteractObjectSystem->ConnectionMatrix[ConnectObject1Index][ConnectObject2Index + Source1Total] = 1;
	NewInteractObjectSystem->ConnectionMatrix[ConnectObject2Index + Source1Total][ConnectObject1Index] = 1;
	//将所有IOI目前的系统设为新系统
	for(AActorInteractObjectInterface* I : NewInteractObjectSystem->TotalInteractObjects)
		I->SetNewInteractObjectSystem();
	//如为玩家拥有将新系统设置为玩家拥有
	if (Source1->IsPossessedByPlayer || Source2->IsPossessedByPlayer)
	{
		NewInteractObjectSystem->IsPossessedByPlayer = true;
		MainPlayerState->PossessedSystem = NewInteractObjectSystem;
	}
	return NewInteractObjectSystem;
}

void AMainGameState::PrintConnectionMatrix(InteractObjectSystem PrintTarget)
{
	FString Tmp_PrintRow;
	FString Tmp_CurrChar;
	UE_LOG(LogTemp,Warning,TEXT("%d"),PrintTarget.TotalInteractObjects.Num());
	for (int i = 0;i < PrintTarget.TotalInteractObjects.Num();i++)
	{
		Tmp_PrintRow = "";
		for(int j = 0;j< PrintTarget.TotalInteractObjects.Num();j++)
		{
			Tmp_CurrChar = FString::FromInt(PrintTarget.ConnectionMatrix[i][j]);
			Tmp_PrintRow += Tmp_CurrChar + " ";
		}
		UE_LOG(LogTemp,Warning,TEXT("%s"),*Tmp_PrintRow)
	}


}
