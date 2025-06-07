// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObjectInterface.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/FTableRowInteractObject.h"
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


void AInteractObjectInterface::UpdateInteractObjectStatus(FString NewInteractObjectNameStatus)
{
	if(InteractObjectDataTable)
	{
		//for(FName RowName : InteractObjectDataTable -> GetRowNames())
		//{
		//UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		this->InteractObjectNameStatus = NewInteractObjectNameStatus;
		FTableRowInteractObject* InteractObjectData =
			InteractObjectDataTable->FindRow<FTableRowInteractObject>(FName(InteractObjectNameStatus),TEXT(""));
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
		}

		//}
	}
}

