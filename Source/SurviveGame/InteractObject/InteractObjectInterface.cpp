// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObjectInterface.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/BehaviorStructure/FTableRowBehavior.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/ScriptStructure/FTableRowScript.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/PropertiesStructure/FTableRowProperty.h"
// Sets default values
AInteractObjectInterface::AInteractObjectInterface()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractObjectInterface::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractObjectInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AInteractObjectInterface::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AInteractObjectInterface::UpdateBehaviorStatus(FString NewItemStatus)
{
	if(BehaviorDataTable)
	{
		//for(FName RowName : InteractObjectDataTable -> GetRowNames())
		//{
		//UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		this->BehaviorStatus = NewItemStatus;
		FTableRowBehavior* InteractObjectData =
			BehaviorDataTable->FindRow<FTableRowBehavior>(FName(BehaviorStatus),TEXT(""));
		if(InteractObjectData)
		{
			Option1 = InteractObjectData->Option1;
			EventID1 = InteractObjectData->EventID1;
			Option2 = InteractObjectData->Option2;
			EventID2 = InteractObjectData->EventID2;
			Option3 = InteractObjectData->Option3;
			EventID3 = InteractObjectData->EventID3;
			Option4 = InteractObjectData->Option4;
			EventID4 = InteractObjectData->EventID4;
			EVENTIDAutoRelease = InteractObjectData->EVENTIDAutoRelease;
		}
	}
}

void AInteractObjectInterface::UpdateInitialProperties(FString NewProperties)
{
	if(PropertiesDataTable)
	{
		this->InitialProperties = NewProperties;
		FTableRowProperty* ScriptData =
			PropertiesDataTable->FindRow<FTableRowProperty>(FName(InitialProperties),TEXT(""));
		if(PropertiesDataTable)
		{
			Property.Weight = ScriptData->Weight;
			Weight = Property.Weight;
		}
	}
}

void AInteractObjectInterface::UpdateCurrentScript(FString NewScript)
{
	if(ScriptDataTable)
	{
		//for(FName RowName : InteractObjectDataTable -> GetRowNames())
		//{
		//UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"),*FPaths::ProjectDir());
		this->CurrentScript = NewScript;
		FTableRowScript* ScriptData =
			ScriptDataTable->FindRow<FTableRowScript>(FName(CurrentScript),TEXT(""));
		if(ScriptDataTable)
		{
			ScriptPath = ScriptData->ScriptPath;
		}
	}
	FString FullScriptPath = FPaths::ProjectDir() + ScriptPath;
	FullScriptPath = FPaths::ConvertRelativePathToFull(FullScriptPath);
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullScriptPath))
	{
		FFileHelper::LoadFileToString(Script,*FullScriptPath);
		UE_LOG(LogTemp,Warning,TEXT("%d"),*Script);
	}
}



