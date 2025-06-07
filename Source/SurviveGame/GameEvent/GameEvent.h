// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurviveGame/InteractObject/InteractObjectInterface.h"
#include "GameEvent.generated.h"


UCLASS()
class SURVIVEGAME_API AGameEvent : public AActor
{
	GENERATED_BODY()
	void (AGameEvent::*CurrentEvent_0_Param)();
	void (AGameEvent::*CurrentEvent_1_IntParam)(int);
public:	
	// Sets default values for this actor's properties
	AGameEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	USceneComponent* SceneComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ReleaseSubEvent(int SubEventID);
	//Targets
	UPROPERTY()
	AActor *MotherObject;
	//EventFlag
	bool EventFlag0 = false;
	bool EventFlag1 = false;
	bool EventFlag2 = false;
	bool EventFlag3 = false;
	bool EventFlag4 = false;
	bool EventFlag5 = false;
	bool EventFlag6 = false;
	bool EventFlag7 = false;
	bool EventFlag8 = false;
	bool EventFlag9 = false;
	//EventParam
	int EventParam0 = 0;
	int EventParam1 = 0;
	int EventParam2 = 0;
	int EventParam3 = 0;
	int EventParam4 = 0;
	//EventID
	UPROPERTY(EditAnywhere)
	int EventID  = 0;
	//Events
	//01 Test
		//IOID 0001
		void ActivlyEvent0100100001();//EventID 00001
		void ActivlyEvent0100100002();//EventID 00001
		void ActivlyEvent0100100003();
	
	//02 AffectInteractObj
	//03 AffectCharacter
	//04 AffectFields
};
