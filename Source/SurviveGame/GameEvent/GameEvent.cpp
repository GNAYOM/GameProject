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

void AGameEvent::MainEventSelector(int ID)
{
	switch (ID)
	{
	case 100100001 : CurrentEvent_0_Param = &AGameEvent::Test00001;
		break;
	case 100100002 : CurrentEvent_0_Param = &AGameEvent::Test00002;
		break;
	case 100100003 : CurrentEvent_0_Param = &AGameEvent::Test00003;
		break;
	case 200000000: CurrentEvent_0_Param = &AGameEvent::ScriptExecutor;
		break;
	case 500000001 :CurrentEvent_0_Param = &AGameEvent::Option1Detection;
		break;
	case 500000002 :CurrentEvent_0_Param = &AGameEvent::Option2Detection;
		break;
	case 500000003 :CurrentEvent_0_Param = &AGameEvent::Option3Detection;
		break;
	case 500000004 :CurrentEvent_0_Param = &AGameEvent::Option4Detection;
		break;
	}
	UE_LOG(LogTemp,Warning,TEXT("%d"),ID);
}

void AGameEvent::BeginPlay()
{
	Super::BeginPlay();
	MainEventSelector(EventID);
}

// Called every frame
void AGameEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentEvent_0_Param != nullptr)
	{
		(this->*CurrentEvent_0_Param)();
	}
	else if (CurrentEvent_1_IntParam != nullptr)
	{
		(this->*CurrentEvent_1_IntParam)(IntEventParam0);
	}
	else if (CurrentEvent_1_StringParam != nullptr)
	{
		(this->*CurrentEvent_1_StringParam)(StringEventParam0);
	}
	UpdateConditionGroupsResult();
}

//01 Test
void AGameEvent::Test00001()
{

	//UE_LOG(LogTemp,Warning,TEXT("Event %d: flag 1000001 set true"),EventID);
	//UE_LOG(LogTemp,Warning,TEXT("Event %d: AND01 true"),EventID);
	//UE_LOG(LogTemp,Warning,TEXT("Event %d: Event End"),EventID);
	//AInteractObjectInterface* tmp_IO = Cast<AInteractObjectInterface>(MotherObject);
	//tmp_IO->UpdateInteractObjectStatus("Test1");
	if(!EventFlag0)
	{
		GameEventSubEventRelease(100100002,this);
		EventFlag0 = true;
	}
	if (EventFlag1)
	{
		UE_LOG(LogTemp,Warning,TEXT("Event %d: Event Destroyed"),EventID);
		Destroy();
		
	}

}

void AGameEvent::Test00002()
{
	
	if(!EventFlag0)
	{
		GameEventSubEventRelease(100100003,this);
		EventFlag0 = true;
	}
	if(EventFlag1)
	{
		Cast<AGameEvent>(MotherObject)->EventFlag1 = true;
		UE_LOG(LogTemp,Warning,TEXT("Event 0100100001: EventFlag1 toggle"));
		UE_LOG(LogTemp,Warning,TEXT("Event %d: Event Destroyed"),EventID);
		Destroy();
	}
	//Tags.Add(FName(TEXT("%d"),EventID));

}
void AGameEvent::Test00003()
{
	UE_LOG(LogTemp,Warning,TEXT("Event 0100100002: EventFlag1 toggle"));
	Cast<AGameEvent>(MotherObject)->EventFlag1 = true;
	UE_LOG(LogTemp,Warning,TEXT("Event %d: Event Destroyed"),EventID);
	Destroy();
	//Tags.Add(FName(TEXT("%d"),EventID));

}


void AGameEvent::ScriptExecutor()
{
	
	if(!CompileFlag)
	{
		ScriptCompiler("S");
		/*ReleaseSubEvent(100100002,this);
		InteractObjectStatusChange("Test1");*/
		CompileFlag = true;
	}
	if(ScriptExecutorPC == ScriptInstructions.Num())
	{Destroy();}
	else
	{
		//03 ItemStatusChange
		if(ScriptInstructions[ScriptExecutorPC].Equals("ItemStatusChange"))//Param(ItemStatus)
		{
				//ReleaseSubEvent(100100002,this);
				ItemStatusChange(ScriptInstructionParams[ScriptExecutorPC][0]);
				ScriptExecutorPC++;
		}
		else if(ScriptInstructions[ScriptExecutorPC].Equals("ItemStatusScriptTrigger"))
		{
			//ReleaseSubEvent(100100002,this);
			ItemStatusChange("ScriptTrigger");
			ScriptExecutorPC++;
		}
		else if(ScriptInstructions[ScriptExecutorPC].Equals("ItemStatusInputDetection"))
		{
			//ReleaseSubEvent(100100002,this);
			ItemStatusChange("InputDetection");
			/*
			bool &test = FlagParamExplaner("ggg");
			test = false;
			UE_LOG(LogTemp,Warning,TEXT("%d"),EventFlag1);	
			*///FlagTest
			ScriptExecutorPC++;
		}
		//08 WaitInput
		else if(ScriptInstructions[ScriptExecutorPC].Equals("ContinueWhenInputDetected"))//Param(Input)
		{
			if(InputExplaner(ScriptInstructionParams[ScriptExecutorPC][0]))
			{
				ScriptExecutorPC++;
			}
		}
		else if(ScriptInstructions[ScriptExecutorPC].Equals("ToggleFlagContinueWhenInputDetected"))//Param(Input,Flag)
		{
			if(InputExplaner(ScriptInstructionParams[ScriptExecutorPC][0]))
			{
				bool& SelectedFlag = FlagParamExplaner(ScriptInstructionParams[ScriptExecutorPC][1]);
				SelectedFlag = !SelectedFlag;
				ScriptExecutorPC++;
			}
		}
		//06 JudgeAsConditionGroup
		else if(ScriptInstructions[ScriptExecutorPC].Equals("JudgeAsConditionGroup"))//Param(ConditionA,ConditionB,ConditionRegister)
		{

			JudgeAsConditionGroup(FlagParamExplaner(ScriptInstructionParams[ScriptExecutorPC][0]),FlagParamExplaner(ScriptInstructionParams[ScriptExecutorPC][1]),OR00);
			JudgeAsConditionGroup(FlagParamExplaner(ScriptInstructionParams[ScriptExecutorPC][0]),FlagParamExplaner(ScriptInstructionParams[ScriptExecutorPC][1]),AND00);
			ScriptExecutorPC++;
		}
		else if(ScriptInstructions[ScriptExecutorPC].Equals("DestroyWhenInputOption1"))//Test
		{
			
			UE_LOG(LogTemp,Warning,TEXT("waitkey"));
			if(MotherIOInterface->Option1JustPressed)
			{Destroy();}
		}
	}
}

//02 ScriptExecutor
void AGameEvent::ScriptCompiler(FString Path)
{
	stack<char> OperatorStack;
	FString TestScript = Cast<AInteractObjectInterface>(MotherObject)->Script;
	FString Tmp_Instruction;
	FString Tmp_InstructionParam;
	TArray<FString> Tmp_InstructionParams;
	for(char I : TestScript)
	{
		
		if(I == '#' || I == '(' ||I == ')')
		{
			OperatorStack.push(I);
		}//PushOperator
		if(OperatorStack.empty())
		{
			continue;
		}//若栈为空直接跳过
		if (I == '#')
		{
			continue;
		}//表示开始进行指令检测
		if(OperatorStack.top() == '#')
		{
			Tmp_Instruction.AppendChar(I);
		}
		if (I == '(')
		{
			ScriptInstructions.Add(Tmp_Instruction);
			UE_LOG(LogTemp,Warning,TEXT("Instruction:%s"),*Tmp_Instruction);
			Tmp_Instruction.Empty();
			continue;
		}//指令检测完毕
		if (OperatorStack.top() == '(')
		{
			if(I == ',')
			{
				Tmp_InstructionParams.Add(Tmp_InstructionParam);
				UE_LOG(LogTemp,Warning,TEXT("InstructionParam:%s"),*Tmp_InstructionParam);
				Tmp_InstructionParam.Empty();
				continue;
			}//若遇到 ',' 直接将检测参数Add入临时Params
			Tmp_InstructionParam.AppendChar(I);
		}
		if(OperatorStack.top() == ')')
		{
			if(Tmp_InstructionParams.IsEmpty() && Tmp_InstructionParam.IsEmpty())
			{
				Tmp_InstructionParam = "NULL";
				Tmp_InstructionParams.Add(Tmp_InstructionParam);
				UE_LOG(LogTemp,Warning,TEXT("InstructionParam:%s"),*Tmp_InstructionParam);
				Tmp_InstructionParam.Empty();
			}//若无参数也需使得ScriptInstructionParams添加NULL,便于与ScriptInstruction一一对应
			Tmp_InstructionParams.Add(Tmp_InstructionParam);
			UE_LOG(LogTemp,Warning,TEXT("InstructionParam:%s"),*Tmp_InstructionParam);
			Tmp_InstructionParam.Empty();
			ScriptInstructionParams.Add(Tmp_InstructionParams);
			Tmp_InstructionParams.Empty();//将最后一组参数加入ScriptInstructionParams
			while (!OperatorStack.empty())
			{
				OperatorStack.pop();
			}//弹出所有操作符
			UE_LOG(LogTemp,Warning,TEXT("OperatorStack empty"));
			UE_LOG(LogTemp,Warning,TEXT("Compile complete"));
		}
	}

}

bool& AGameEvent::FlagParamExplaner(FString Flag)
{
	//EventFlag
	if(Flag.Equals("Flag0"))
		return EventFlag0;

	else if(Flag.Equals("Flag1"))
		return EventFlag1;
	
	else if(Flag.Equals("Flag2"))
		return EventFlag2;
	
	else if(Flag.Equals("Flag3"))
		return EventFlag3;
	
	else if(Flag.Equals("Flag4"))
		return EventFlag4;
	
	else if(Flag.Equals("Flag5"))
		return EventFlag5;
	
	else if(Flag.Equals("Flag6"))
		return EventFlag6;
	
	else if(Flag.Equals("Flag7"))
		return EventFlag7;
	
	else if(Flag.Equals("Flag8"))
		return EventFlag8;
	
	else if(Flag.Equals("Flag9"))
		return EventFlag9;
	else
		return EventFlag0;
}

bool AGameEvent::InputExplaner(FString Input)
{
	bool InputStatus = false;
	if(Input.Equals("Option1"))
		InputStatus = MotherIOInterface->Option1JustPressed;
	
	else if(Input.Equals("Option2"))
		InputStatus = MotherIOInterface->Option2JustPressed;
	
	else if(Input.Equals("Option3"))
		InputStatus = MotherIOInterface->Option3JustPressed;
	
	else if(Input.Equals("Option4"))
		InputStatus = MotherIOInterface->Option4JustPressed;
	return InputStatus;
}

//03 ItemStatusChange
void AGameEvent::ItemStatusChange(FString NewStatus)//只有最外层事件可以调用
{
	MotherIOInterface->UpdateItemStatus(NewStatus);
}

//04 EventRelease
void AGameEvent::GameEventSubEventRelease(int SubEventID,AGameEvent* FatherObject)
{
	AGameEvent* tmp_NewGameEvent =Cast<AGameEvent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AGameEvent::StaticClass(), FTransform::Identity,ESpawnActorCollisionHandlingMethod::Undefined,this)); 
	tmp_NewGameEvent->EventID = SubEventID;
	tmp_NewGameEvent->MotherGameEvent = this;
	UGameplayStatics::FinishSpawningActor(tmp_NewGameEvent,FTransform::Identity);
	tmp_NewGameEvent->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
}
//05 InputDetection
void AGameEvent::Option1Detection()
{
	UE_LOG(LogTemp,Warning,TEXT("OP1PRESSED"));
	MotherIOInterface->Option1JustPressed = true;
	Destroy();
}

void AGameEvent::Option2Detection()
{
	UE_LOG(LogTemp,Warning,TEXT("OP2PRESSED"));
	MotherIOInterface->Option2JustPressed = true;
	Destroy();
}

void AGameEvent::Option3Detection()
{
	UE_LOG(LogTemp,Warning,TEXT("OP3PRESSED"));
	MotherIOInterface->Option3JustPressed = true;
	Destroy();
}

void AGameEvent::Option4Detection()
{
	UE_LOG(LogTemp,Warning,TEXT("OP4PRESSED"));
	MotherIOInterface->Option4JustPressed = true;
	Destroy();
}
//06 ConditionJudgement
void AGameEvent::JudgeAsConditionGroup(bool &A,bool &B,AndConditionGroup &AND)
{
	AND.ConditionA = &A;
	AND.ConditionB = &B;
	AND.JudgeStart = true;
}

void AGameEvent::JudgeAsConditionGroup(bool &A,bool &B,OrConditionGroup &OR)
{
	OR.ConditionA = &A;
	OR.ConditionB = &B;
	OR.JudgeStart = true;
}

void AGameEvent::AndGroupGetResult(AndConditionGroup &AndConditionGroup)
{
	if(AndConditionGroup.JudgeStart)
	{
		AndConditionGroup.Result = *AndConditionGroup.ConditionA && *AndConditionGroup.ConditionB;
		UE_LOG(LogTemp,Warning,TEXT("%d"),*AndConditionGroup.ConditionA);
		UE_LOG(LogTemp,Warning,TEXT("%d"),*AndConditionGroup.ConditionB)
	}

}

void AGameEvent::OrGroupGetResult(OrConditionGroup &OrConditionGroup)
{
	if(OrConditionGroup.JudgeStart)
	{
		OrConditionGroup.Result = *OrConditionGroup.ConditionA || *OrConditionGroup.ConditionB;
		UE_LOG(LogTemp,Warning,TEXT("%d"),*OrConditionGroup.ConditionA);
		UE_LOG(LogTemp,Warning,TEXT("%d"),*OrConditionGroup.ConditionB)
	}

	
}

void AGameEvent::UpdateConditionGroupsResult()
{
	//UpdateANDRegister
	AndGroupGetResult(AND00);
	if(AND00.Result)
		UE_LOG(LogTemp,Warning,TEXT("AND00:true"));
	AndGroupGetResult(AND01);
	if(AND01.Result)
		UE_LOG(LogTemp,Warning,TEXT("AND01:true"));
	AndGroupGetResult(AND02);
	if(AND02.Result)
		UE_LOG(LogTemp,Warning,TEXT("AND02:true"));
	AndGroupGetResult(AND03);
	if(AND03.Result)
		UE_LOG(LogTemp,Warning,TEXT("AND03:true"));
	//UpdateORRegister
	OrGroupGetResult(OR00);
	if(OR00.Result)
		UE_LOG(LogTemp,Warning,TEXT("OR00:true"));
	OrGroupGetResult(OR01);
	if(OR01.Result)
		UE_LOG(LogTemp,Warning,TEXT("OR01:true"));
	OrGroupGetResult(OR02);
	if(OR02.Result)
		UE_LOG(LogTemp,Warning,TEXT("OR02:true"));
	OrGroupGetResult(OR03);
	if(OR03.Result)
		UE_LOG(LogTemp,Warning,TEXT("OR03:true"));
}
//08 WaitInput




