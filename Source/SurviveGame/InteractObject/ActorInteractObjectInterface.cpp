// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInteractObjectInterface.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/BehaviorStructure/FTableRowBehavior.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/ScriptStructure/FTableRowScript.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/PropertiesStructure/FTableRowProperty.h"
// Sets default values
AActorInteractObjectInterface::AActorInteractObjectInterface()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentSocketInfo = TargetSocketInfo();
}

// Called when the game starts or when spawned
void AActorInteractObjectInterface::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorInteractObjectInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input


void AActorInteractObjectInterface::UpdateBehaviorStatus(FString NewItemStatus)
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

void AActorInteractObjectInterface::UpdateInitialProperties(FString NewProperties)
{
	if(PropertiesDataTable)
	{
		this->InitialProperties = NewProperties;
		FTableRowProperty* ScriptData =
			PropertiesDataTable->FindRow<FTableRowProperty>(FName(InitialProperties),TEXT(""));
		if(ScriptData)
		{
			Property.Weight = ScriptData->Weight;
			Weight = Property.Weight;
		}
	}
}

void AActorInteractObjectInterface::UpdateCurrentScript(FString NewScript)
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
		if(ScriptData)
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

void AActorInteractObjectInterface::AutoReleaseEvent()
{
}

void AActorInteractObjectInterface::ReleaseEventActively(int Input)
{
}

FVector AActorInteractObjectInterface::GetPlayerBackSocketPosition()
{
	return FVector(0,0,0);
}

FRotator AActorInteractObjectInterface::GetPlayerDirectionRotator()
{
	return FRotator(0,0,0);
}

FVector AActorInteractObjectInterface::GetPlayerRHandSocketPosition()
{
	return FVector(0,0,0);
}

UStaticMeshComponent* AActorInteractObjectInterface::GetPlayerBackSocketComponent()
{
	UStaticMeshComponent* Default = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default"));
	return Default;
}

void AActorInteractObjectInterface::SetPlayerPossessedInteractObjectSystem()
{
	
}

bool AActorInteractObjectInterface::MergeWithPlayerPossessedInteractObjectSystem()
{
	return false;
}

void AActorInteractObjectInterface::MergeWithSourceInteractObject()
{
}

void AActorInteractObjectInterface::SeperateFromPlayerPossessedInteractObjectSystem()
{
}

void AActorInteractObjectInterface::SeperateFromSourceInteractObject()
{
}

void AActorInteractObjectInterface::SetNewInteractObjectSystem()
{
}

void AActorInteractObjectInterface::SetConnectionWithInteractObject()
{
}

void AActorInteractObjectInterface::SetAsPlayerBlockingEquippedInteractObject()
{
	
}

void AActorInteractObjectInterface::ClearPlayerBlockingEquippedInteractObject()
{
}

void AActorInteractObjectInterface::UseEquipment()
{
}

void AActorInteractObjectInterface::InitSocketPanel()
{
}

void AActorInteractObjectInterface::LogHello()
{
	UE_LOG(LogTemp,Warning,TEXT("Hello  ::)"));
}





