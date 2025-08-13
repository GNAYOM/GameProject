// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurviveGame/InteractObject/ActorInteractObjectInterface.h"
#include "stack"
#include "SurviveGame/Enum/InteractObjectStatus/InteractObjectStatus.h"
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
	AActorInteractObjectInterface *MotherIOInterface;
	UPROPERTY()
	AGameEvent *MotherGameEvent;
	//ConditionRegister
	AndConditionGroup DefaultAND;
	AndConditionGroup AND00;
	AndConditionGroup AND01;
	AndConditionGroup AND02;
	AndConditionGroup AND03;
	OrConditionGroup DefaultOR;
	OrConditionGroup OR00;
	OrConditionGroup OR01;
	OrConditionGroup OR02;
	OrConditionGroup OR03;
	//EventFlag
	bool CompileFlag = false;
	bool JumpFlag = false;
	bool DefaultFlag = false;
	bool EventFlagTrue = true;
	bool EventFlagFalse = false;
	bool IsConnected = false;
	bool IsEquipped = false;
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
	//ConditionRegisterInitialize
	void ConditionRegisterInitialize();
	//InteractObjectInputFlagRefresh
	void InteractObjectInputFlagRefresh();
	//Events
	//01 Test
		//IOID 0001
		void Test00001();//EventID 00001
		void Test00002();//EventID 00002
		void Test00003();//EventID 00003
	//002 ScriptExecutor
		void ScriptExecutor();//EventID 002000000
		void ScriptCompiler(FString Path);
		FString GetPCInstruction();
		FString GetPCParam(int ParamIndex);
		bool& FlagParamExplainer(FString Flag);
		AndConditionGroup& AndConditionGroupParamExplainer(FString AndConditionGroup);
		OrConditionGroup& OrConditionGroupParamExplainer(FString OrConditionGroup);
		bool InputExplainer(FString Input);
		TArray<FString>	ScriptInstructions;
		TArray<TArray<FString>> ScriptInstructionParams;
		FString ScriptPath;
		int ScriptExecutorPC=0;
		//int ScriptExecutorIR;
	//003 BehaviorStatusChange
		void BehaviorStatusChange(FString NewStatus);
	//004 EventRelease
		void GameEventSubEventRelease(int SubEventID,AGameEvent* FatherObject);
	//005 InputDetection
		void Option1Detection();//ID000001
		void Option2Detection();//ID000002
		void Option3Detection();//ID000003
		void Option4Detection();//ID000004
	//006 ConditionJudgement
		void JudgeAsConditionGroup(bool &A,bool &B,AndConditionGroup &AND);
		void JudgeAsConditionGroup(bool &A,bool &B,OrConditionGroup &OR);
		void AndGroupGetResult(AndConditionGroup &AndConditionGroup);
		void OrGroupGetResult(OrConditionGroup &OrConditionGroup);
		void UpdateConditionGroupsResult();
	//007 ConditionalConstruct
	//008 WaitInput
	//009 CollectableItem
		void CollectableBehavior();//EventID 009000000
	//010 BackStorage
		void EquipableBackStorageBehavior();//EventID 010000000
	//011 InteractObjectStatusChange
		void MotherIOInterfaceChangeStatus(int Status);
		int  GetMotherIOInterfaceStatus();
	// AffectCharacter
	// AffectFields
};
