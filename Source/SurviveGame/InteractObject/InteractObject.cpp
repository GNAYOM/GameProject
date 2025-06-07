// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject.h"

#include "Kismet/GameplayStatics.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/FTableRowInteractObject.h"

// Sets default values
AInteractObject::AInteractObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractObjectVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractObjectVolume"));
	InteractObjectVolume -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractObjectVolume -> SetCollisionProfileName(FName(TEXT("InteractObject")));
	InteractObjectVolume ->SetupAttachment(RootComponent);
	PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	PrimitiveComponent -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimitiveComponent -> SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	PrimitiveComponent -> OnComponentBeginOverlap.AddDynamic(this,&AInteractObject::OnCamBeginOverlap);
	PrimitiveComponent -> OnComponentEndOverlap.AddDynamic(this,&AInteractObject::OnCamEndOverlapEnd);
}

// Called when the game starts or when spawned
void AInteractObject::BeginPlay()
{
	Super::BeginPlay(); 
	/*InteractObjectDataTable = LoadObject<UDataTable>(this
		,TEXT("/Script/Engine.DataTable'/Game/GameContent/DataTable/InterractObject/InteractObjectDataTable.InteractObjectDataTable'"));*/
	if(InteractObjectDataTable)
	{
		//for(FName RowName : InteractObjectDataTable -> GetRowNames())
		//{
			//UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
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

void AInteractObject::ReleaseEventActively(int OptionSlotNum)
{
	//选项槽转事件ID
	int tmp_InEventID;
	switch (OptionSlotNum)
	{
	case 1: tmp_InEventID = EventID1;
		break;
	case 2: tmp_InEventID = EventID2;
		break;
	case 3: tmp_InEventID = EventID3;
		break;
	case 4: tmp_InEventID = EventID4;
		break;
	}
	//延迟 spawn
	AGameEvent* tmp_NewGameEvent =Cast<AGameEvent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AGameEvent::StaticClass(), FTransform::Identity,ESpawnActorCollisionHandlingMethod::Undefined,this)); 
	tmp_NewGameEvent->EventID = tmp_InEventID;
	tmp_NewGameEvent->MotherObject = this;
	UGameplayStatics::FinishSpawningActor(tmp_NewGameEvent,FTransform::Identity);
	tmp_NewGameEvent->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	
}

void AInteractObject::UpdateInteractObjectStatus(FString NewInteractObjectNameStatus)
{
	Super::UpdateInteractObjectStatus(NewInteractObjectNameStatus);
}


void AInteractObject::OnCamBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("BeginOverLap"));
	
	PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimitiveComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Block);
	
}

void AInteractObject::OnCamEndOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
	PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimitiveComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
}

// Called every frame
void AInteractObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




// Called to bind functionality to input
void AInteractObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

