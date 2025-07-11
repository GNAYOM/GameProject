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
	InteractObjectSystems.Remove(Source1);
	InteractObjectSystems.Remove(Source2);
	InteractObjectSystems.Add(NewInteractObjectSystem);
	return NewInteractObjectSystem;
}

InteractObjectSystem* AMainGameState::SeperateInteractObjectSystem(InteractObjectSystem* Source1,
		int DisConnectObject1Index,int DisConnectObject2Index)
{
	int Source1Total = Source1->TotalInteractObjects.Num();
	int NewSystem1Total = 0;
	int NewSystem2Total = 0;
	Source1->ConnectionMatrix[DisConnectObject1Index][DisConnectObject2Index] = 0;
	Source1->ConnectionMatrix[DisConnectObject2Index][DisConnectObject1Index] = 0;
	
	InteractObjectSystem* NewInteractObjectSystem1 = new InteractObjectSystem();
	InteractObjectSystem* NewInteractObjectSystem2 = new InteractObjectSystem();
	
	std::queue<int> Tmp_BfsQueue;
	TArray<bool> visited;
	TArray<int> NewSystem1FromSource1;
	TArray<int> NewSystem2FromSource1;
	for(int i = 0;i<Source1Total;i++)
		visited.Add(false);
	Tmp_BfsQueue.push(DisConnectObject1Index);
	while(!Tmp_BfsQueue.empty())
	{
		int Tmp_CurrentVisitIndex = Tmp_BfsQueue.front();
		Tmp_BfsQueue.pop();
		visited[Tmp_CurrentVisitIndex] = true;
		NewSystem1FromSource1.Add(Tmp_CurrentVisitIndex);
		NewInteractObjectSystem1->TotalInteractObjects.Add(Source1->TotalInteractObjects[Tmp_CurrentVisitIndex]);
		for (int i = 0; i<Source1Total;i++)//遍历目前访问结点所属行，寻找所有有连接的结点
		{
			if(Source1->ConnectionMatrix[Tmp_CurrentVisitIndex][i])//如果当前访问点与目标点有连接
				if(!visited[i])
					Tmp_BfsQueue.push(i);
		}
	}
	Tmp_BfsQueue.push(DisConnectObject2Index);
	while(!Tmp_BfsQueue.empty())
	{
		int Tmp_CurrentVisitIndex = Tmp_BfsQueue.front();
		Tmp_BfsQueue.pop();
		visited[Tmp_CurrentVisitIndex] = true;
		NewSystem2FromSource1.Add(Tmp_CurrentVisitIndex);
		NewInteractObjectSystem2->TotalInteractObjects.Add(Source1->TotalInteractObjects[Tmp_CurrentVisitIndex]);
		for (int i = 0; i<Source1Total;i++)//遍历目前访问结点所属行，寻找所有有连接的结点
		{
			if(Source1->ConnectionMatrix[Tmp_CurrentVisitIndex][i])//如果当前访问点与目标点有连接
				if(!visited[i])
					Tmp_BfsQueue.push(i);
		}
	}

	NewSystem1Total = NewSystem1FromSource1.Num();
	NewSystem2Total = NewSystem2FromSource1.Num();
	
	if (NewSystem1Total == Source1Total)
	{
		return Source1;
	}
	
	//设置行格式
	TArray<int> SystemConnectionMatrixNewRow;
	for (int i = 0;i < Source1Total;i++)
		SystemConnectionMatrixNewRow.Add(0);
	//初始化连接矩阵
	for (int i = 0;i < NewSystem1Total;i++)
		NewInteractObjectSystem1->ConnectionMatrix.Add(SystemConnectionMatrixNewRow);
	//复制连接矩阵
	for (int i = 0;i < NewSystem1Total;i++)
		for(int j = 0;j<Source1Total;j++)
			NewInteractObjectSystem1->ConnectionMatrix[i][j] = Source1->ConnectionMatrix[NewSystem1FromSource1[i]][j];
	
	for (int i = 0;i < NewSystem1Total;i++)
		for(int j = Source1Total - 1;j>=0;j--)
			if(NewSystem2FromSource1.Find(j) != -1)
				NewInteractObjectSystem1->ConnectionMatrix[i].RemoveAt(j);
	//初始化连接矩阵
	for (int i = 0;i < NewSystem2Total;i++)
		NewInteractObjectSystem2->ConnectionMatrix.Add(SystemConnectionMatrixNewRow);
	//复制连接矩阵
	for (int i = 0;i < NewSystem2Total;i++)
		for(int j = 0;j<Source1Total;j++)
			NewInteractObjectSystem2->ConnectionMatrix[i][j] = Source1->ConnectionMatrix[NewSystem2FromSource1[i]][j];
	for (int i = 0;i < NewSystem2Total;i++)
		for(int j = Source1Total - 1;j>=0;j--)
			if(NewSystem1FromSource1.Find(j) != -1)
				NewInteractObjectSystem2->ConnectionMatrix[i].RemoveAt(j);
	
	//将所有IOI目前的系统设为新系统
	for(AActorInteractObjectInterface* I : NewInteractObjectSystem1->TotalInteractObjects)
	{
		Source1->NewInteractObjectSystem = NewInteractObjectSystem1;
		I->SetNewInteractObjectSystem();
	}

	for(AActorInteractObjectInterface* I : NewInteractObjectSystem2->TotalInteractObjects)
	{
		Source1->NewInteractObjectSystem = NewInteractObjectSystem2;
		I->SetNewInteractObjectSystem();
	}
	//若存在Equipped类behavior，设为玩家拥有
	for(AActorInteractObjectInterface* I : NewInteractObjectSystem1->TotalInteractObjects)
	{
		if(I->BehaviorStatus.Equals("EquippedBackStorage"))
		{
			NewInteractObjectSystem1->IsPossessedByPlayer = true;
			MainPlayerState->PossessedSystem = NewInteractObjectSystem1;
			break;
		}
	}
	for(AActorInteractObjectInterface* I : NewInteractObjectSystem2->TotalInteractObjects)
	{
		if(I->BehaviorStatus.Equals("EquippedBackStorage"))
		{
			NewInteractObjectSystem2->IsPossessedByPlayer = true;
			MainPlayerState->PossessedSystem = NewInteractObjectSystem2;
			break;
		}
	}
	InteractObjectSystems.Remove(Source1);
	InteractObjectSystems.Add(NewInteractObjectSystem1);
	InteractObjectSystems.Add(NewInteractObjectSystem2);
	
	return NewInteractObjectSystem1;
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
