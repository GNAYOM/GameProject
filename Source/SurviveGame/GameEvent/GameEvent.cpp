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
}

void AGameEvent::ItemStatusChange(FString NewStatus)//只有最外层事件可以调用
{
	MotherIOInterface->UpdateItemStatus(NewStatus);
}

void AGameEvent::ReleaseSubEvent(int SubEventID,AGameEvent* FatherObject)
{
	AGameEvent* tmp_NewGameEvent =Cast<AGameEvent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AGameEvent::StaticClass(), FTransform::Identity,ESpawnActorCollisionHandlingMethod::Undefined,this)); 
	tmp_NewGameEvent->EventID = SubEventID;
	tmp_NewGameEvent->MotherObject = this;
	UGameplayStatics::FinishSpawningActor(tmp_NewGameEvent,FTransform::Identity);
	tmp_NewGameEvent->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
}

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

void AGameEvent::Test00001()
{

	//UE_LOG(LogTemp,Warning,TEXT("Event %d: flag 1000001 set true"),EventID);
	//UE_LOG(LogTemp,Warning,TEXT("Event %d: AND01 true"),EventID);
	//UE_LOG(LogTemp,Warning,TEXT("Event %d: Event End"),EventID);
	//AInteractObjectInterface* tmp_IO = Cast<AInteractObjectInterface>(MotherObject);
	//tmp_IO->UpdateInteractObjectStatus("Test1");
	if(!EventFlag0)
	{
		ReleaseSubEvent(100100002,this);
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
		ReleaseSubEvent(100100003,this);
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
	
	if(!EventFlag0)
	{
		ScriptCompiler("S");
		/*ReleaseSubEvent(100100002,this);
		InteractObjectStatusChange("Test1");*/
		EventFlag0 = true;
	}
	if(ScriptExecutorPC == ScriptInstructions.Num())
	{Destroy();}
	else
	{
		if(ScriptInstructions[ScriptExecutorPC].Equals("ItemStatusChange"))
		{
				//ReleaseSubEvent(100100002,this);
				ItemStatusChange(ScriptInstructionParams[ScriptExecutorPC][0]);
				ScriptExecutorPC++;
		}
		if(ScriptInstructions[ScriptExecutorPC].Equals("DestroyWhenInputOption1"))
		{
			UE_LOG(LogTemp,Warning,TEXT("waitkey"));
			if(MotherIOInterface->Option1JustPressed)
			{Destroy();}
		}
	}
}


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

