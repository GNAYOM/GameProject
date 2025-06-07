// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEvent.h"

#include "ComponentUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameEvent::AGameEvent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned

void AGameEvent::BeginPlay()
{
	Super::BeginPlay();
	
	switch (EventID)
	{
	case 100100001 : CurrentEvent_0_Param = &AGameEvent::ActivlyEvent0100100001;
		break;
	case 100100002 : CurrentEvent_0_Param = &AGameEvent::ActivlyEvent0100100002;
		break;
	case 100100003 : CurrentEvent_0_Param = &AGameEvent::ActivlyEvent0100100003;
		break;
	}
	UE_LOG(LogTemp,Warning,TEXT("%d"),EventID);

}

// Called every frame
void AGameEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentEvent_0_Param != nullptr)
	{
		(this->*CurrentEvent_0_Param)();
	}
	if (CurrentEvent_1_IntParam != nullptr)
	{
		(this->*CurrentEvent_1_IntParam)(EventParam1);
	}
}

void AGameEvent::ReleaseSubEvent(int SubEventID)
{
	AGameEvent* tmp_NewGameEvent =Cast<AGameEvent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AGameEvent::StaticClass(), FTransform::Identity,ESpawnActorCollisionHandlingMethod::Undefined,this)); 
	tmp_NewGameEvent->EventID = SubEventID;
	tmp_NewGameEvent->MotherObject = this;
	UGameplayStatics::FinishSpawningActor(tmp_NewGameEvent,FTransform::Identity);
	tmp_NewGameEvent->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
}

void AGameEvent::ActivlyEvent0100100001()
{
	UE_LOG(LogTemp,Warning,TEXT("Event %d released activly"),EventID);
	UE_LOG(LogTemp,Warning,TEXT("Event %d: Event Begin"),EventID);
	//UE_LOG(LogTemp,Warning,TEXT("Event %d: flag 1000001 set true"),EventID);
	//UE_LOG(LogTemp,Warning,TEXT("Event %d: AND01 true"),EventID);
	//UE_LOG(LogTemp,Warning,TEXT("Event %d: Event End"),EventID);
	AInteractObjectInterface* tmp_IO = Cast<AInteractObjectInterface>(MotherObject);
	//tmp_IO->UpdateInteractObjectStatus("Test1");
	if(!EventFlag0)
	{
		ReleaseSubEvent(100100002);
		EventFlag0 = true;
	}
	if (EventFlag1)
	{
		Destroy();
	}

}

void AGameEvent::ActivlyEvent0100100002()
{
	UE_LOG(LogTemp,Warning,TEXT("Event %d released activly"),EventID);
	UE_LOG(LogTemp,Warning,TEXT("Event %d: Event Begin"),EventID);
	if(!EventFlag0)
	{
		ReleaseSubEvent(100100003);
		EventFlag0 = true;
	}
	if(EventFlag1)
	{
		Cast<AGameEvent>(MotherObject)->EventFlag1 = true;
		Destroy();
	}
	//Tags.Add(FName(TEXT("%d"),EventID));

}
void AGameEvent::ActivlyEvent0100100003()
{
	UE_LOG(LogTemp,Warning,TEXT("Event %d released activly"),EventID);
	UE_LOG(LogTemp,Warning,TEXT("Event %d: Event Begin"),EventID);
	Cast<AGameEvent>(MotherObject)->EventFlag1 = true;
	Destroy();
	//Tags.Add(FName(TEXT("%d"),EventID));

}
