// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurviveGame/InteractObject/InteractObjectInterface.h"
#include "stack"
#include "GameEvent.generated.h"

using namespace std;

UCLASS()
class SURVIVEGAME_API AGameEvent : public AActor
{
	GENERATED_BODY()
	void (AGameEvent::*CurrentEvent_0_Param)();
	void (AGameEvent::*CurrentEvent_1_IntParam)(int);
	void (AGameEvent::*CurrentEvent_1_StringParam)(FString);
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

	//Targets
	UPROPERTY()
	AActor *MotherObject;
	AInteractObjectInterface *MotherIOInterface;
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
	int IntEventParam0 = 0;
	int IntEventParam1 = 0;
	int IntEventParam2 = 0;
	int IntEventParam3 = 0;
	int IntEventParam4 = 0;
	
	FString StringEventParam0 = "";
	FString StringEventParam1 = "";
	FString StringEventParam2 = "";
	FString StringEventParam3 = "";
	FString StringEventParam4 = "";
	//EventID
	UPROPERTY(EditAnywhere)
	int EventID  = 0;
	//EventSelector
	void MainEventSelector(int ID);
	//Events
	//01 Test
		//IOID 0001
		void Test00001();//EventID 00001
		void Test00002();//EventID 00002
		void Test00003();//EventID 00003
	//02 ScriptExecutor
		void ScriptExecutor();//EventID 200000000
		void ScriptCompiler(FString Path);
		TArray<FString>	ScriptInstructions;
		TArray<TArray<FString>> ScriptInstructionParams;
		FString ScriptPath;
		int ScriptExecutorPC=0;
		int ScriptExecutorIR;
	//03 InteractorObjectStatusChange
		void ItemStatusChange(FString NewStatus);	
	//04 SubEventRelease
		void ReleaseSubEvent(int SubEventID,AGameEvent* FatherObject);
	//05 InputDetection
		void Option1Detection();//ID0000_1
		void Option2Detection();//ID00002
		void Option3Detection();//ID00003
		void Option4Detection();//ID00004
	// AffectCharacter
	// AffectFields
};
