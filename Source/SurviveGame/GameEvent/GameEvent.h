// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurviveGame/InteractObject/InteractObjectInterface.h"
#include "stack"
#include "GameEvent.generated.h"

using namespace std;

struct AndConditionGroup
{
	bool *ConditionA;
	bool *ConditionB;
	bool Result = false;
	bool JudgeStart;
};

struct OrConditionGroup
{
	bool *ConditionA;
	bool *ConditionB;
	bool Result = false;
	bool JudgeStart;
};
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
	UPROPERTY()
	AInteractObjectInterface *MotherIOInterface;
	UPROPERTY()
	AGameEvent *MotherGameEvent;
	//ConditionRegister
	AndConditionGroup AND00 ;
	AndConditionGroup AND01 ;
	AndConditionGroup AND02 ;
	AndConditionGroup AND03 ;
	OrConditionGroup OR00 ;
	OrConditionGroup OR01 ;
	OrConditionGroup OR02 ;
	OrConditionGroup OR03 ;
	//EventFlag
	bool CompileFlag = false;
	bool DefaultFlag = false;
	bool EventFlag0 = false;
	bool EventFlag1 = true;
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
	//ConditionRegisterInitialize
	void ConditionRegisterInitialize();
	//Events
	//01 Test
		//IOID 0001
		void Test00001();//EventID 00001
		void Test00002();//EventID 00002
		void Test00003();//EventID 00003
	//02 ScriptExecutor
		void ScriptExecutor();//EventID 200000000
		void ScriptCompiler(FString Path);
		bool& FlagParamExplaner(FString Flag);
		bool InputExplaner(FString Input);
		TArray<FString>	ScriptInstructions;
		TArray<TArray<FString>> ScriptInstructionParams;
		FString ScriptPath;
		int ScriptExecutorPC=0;
		int ScriptExecutorIR;
	//03 ItemStatusChange
		void ItemStatusChange(FString NewStatus);
	//04 EventRelease
		void GameEventSubEventRelease(int SubEventID,AGameEvent* FatherObject);
	//05 InputDetection
		void Option1Detection();//ID00001
		void Option2Detection();//ID00002
		void Option3Detection();//ID00003
		void Option4Detection();//ID00004
	//06 ConditionJudgement
		void JudgeAsConditionGroup(bool &A,bool &B,AndConditionGroup &AND);
		void JudgeAsConditionGroup(bool &A,bool &B,OrConditionGroup &OR);
		void AndGroupGetResult(AndConditionGroup &AndConditionGroup);
		void OrGroupGetResult(OrConditionGroup &OrConditionGroup);
		void UpdateConditionGroupsResult();
	//07 ConditionalConstruct
	//08 WaitInput
	// AffectCharacter
	// AffectFields
};
