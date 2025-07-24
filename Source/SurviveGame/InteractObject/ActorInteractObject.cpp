// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInteractObject.h"
#include "Kismet/GameplayStatics.h"
#include "SurviveGame/DataStructure/InteractObjectStructure/BehaviorStructure/FTableRowBehavior.h"
#include "SurviveGame/GameCharacter/BaseCharacter/BaseCharacter.h"

// Sets default values
AActorInteractObject::AActorInteractObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	RootComponent = Collision;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent ->SetupAttachment(RootComponent);
	PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	A = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PC"));
	A->SetupAttachment(MeshComponent);
	//Cast<UPrimitiveComponent>(GetRootComponent())->SetCollisionProfileName(FName("BlockAll"));
	InteractObjectVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractObjectVolume"));
	InteractObjectVolume -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractObjectVolume -> SetCollisionProfileName(FName(TEXT("InteractObject")));
	InteractObjectVolume ->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AActorInteractObject::BeginPlay()
{
	Super::BeginPlay(); 
	/*InteractObjectDataTable = LoadObject<UDataTable>(this
		,TEXT("/Script/Engine.DataTable'/Game/GameContent/DataTable/InterractObject/InteractObjectDataTable.InteractObjectDataTable'"));*/
	MainGameState = GetWorld()->GetAuthGameMode()->GetGameState<AMainGameState>();
	MainPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMainPlayerState>(); 
	UpdateBehaviorStatus(BehaviorStatus);
	UpdateCurrentScript(CurrentScript);
	UpdateInitialProperties(InitialProperties);
	if(EVENTIDAutoRelease != 0)
		AutoReleaseEvent();
	PrimitiveComponent -> SetSimulatePhysics(true);
	//PrimitiveComponent -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimitiveComponent -> SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	//PrimitiveComponent -> SetCollisionProfileName(FName(""));
	PrimitiveComponent -> OnComponentBeginOverlap.AddDynamic(this,&AActorInteractObject::OnCamBeginOverlap);
	PrimitiveComponent -> OnComponentEndOverlap.AddDynamic(this,&AActorInteractObject::OnCamEndOverlapEnd);
	//PrimaryInteractObjectSystem
	CurrentInteractObjectSystem =  MainGameState->CreateInteractObjectSystem(this);
	//MainGameState->InteractObjectSystemAddInteractObject(CurrentInteractObjectSystem,this);
	//UE_LOG(LogTemp,Warning,TEXT("%d"),CurrentInteractObjectSystem->ConnectionMatrix[1][1]);
	//UE_LOG(LogTemp,Warning,TEXT("%d"),CurrentInteractObjectSystem->TotalInteractObjects.Num());
	InitSocketPanel();
	//UE_LOG(LogTemp,Warning,TEXT("%d"),	MainGameState->InteractObjectSystems[MainGameState->InteractObjectSystems.Find(CurrentInteractObjectSystem)]->ConnectionMatrix[1][1]);
	//UE_LOG(LogTemp,Warning,TEXT("%d"),	MainGameState->InteractObjectSystems[MainGameState->InteractObjectSystems.Find(CurrentInteractObjectSystem)]->TotalInteractObjects.Num());
}

void AActorInteractObject::AutoReleaseEvent()
{
	//延迟 spawn
	AGameEvent* tmp_NewGameEvent =Cast<AGameEvent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AGameEvent::StaticClass(), FTransform::Identity,ESpawnActorCollisionHandlingMethod::Undefined,this)); 
	tmp_NewGameEvent->EventID = EVENTIDAutoRelease;
	tmp_NewGameEvent->MotherObject = this;
	tmp_NewGameEvent->MotherIOInterface = Cast<AActorInteractObjectInterface>(this);
	UGameplayStatics::FinishSpawningActor(tmp_NewGameEvent,FTransform::Identity);
	tmp_NewGameEvent->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
}


void AActorInteractObject::ReleaseEventActively(int Input)
{
	//选项槽转事件ID
	int tmp_InEventID;
	switch (Input)
	{
	case InputEnum::Option1: tmp_InEventID = EventID1;
		break;
	case InputEnum::Option2: tmp_InEventID = EventID2;
		break;
	case InputEnum::Option3: tmp_InEventID = EventID3;
		break;
	case InputEnum::Option4: tmp_InEventID = EventID4;
		break;
	}
	//延迟 spawn
	AGameEvent* tmp_NewGameEvent =Cast<AGameEvent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AGameEvent::StaticClass(), FTransform::Identity,ESpawnActorCollisionHandlingMethod::Undefined,this)); 
	tmp_NewGameEvent->EventID = tmp_InEventID;
	tmp_NewGameEvent->MotherObject = this;
	tmp_NewGameEvent->MotherIOInterface = this;
	UGameplayStatics::FinishSpawningActor(tmp_NewGameEvent,FTransform::Identity);
	tmp_NewGameEvent->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	//Cast<UPrimitiveComponent>(GetRootComponent())->SetSimulatePhysics(false);
	//SetActorLocation(MainPlayerState->BackSocketCurrentLocation);
	//SetActorRotation(MainPlayerState->CurrentDirectionNormal.Rotation());
	//UE_LOG(LogTemp,Warning,TEXT("x %f,y %f,z %f"),MainPlayerState->BackSocketCurrentLocation.X,MainPlayerState->BackSocketCurrentLocation.Y,MainPlayerState->BackSocketCurrentLocation.Z);
	//AActor* MainCharacter = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	//AttachToComponent(MainCharacter->FindComponentByClass<UStaticMeshComponent>(),FAttachmentTransformRules::KeepWorldTransform);
	//UPhysicsConstraintComponent* TMP = MainCharacter->FindComponentByClass<UPhysicsConstraintComponent>();
	//PrimitiveComponent -> SetSimulatePhysics(false);

	//A->UpdateConstraintFrames();
	/*UPhysicsConstraintComponent* Aa =Cast<UPhysicsConstraintComponent>( AddComponentByClass(UPhysicsConstraintComponent::StaticClass()
		,true
		,GetTransform(),true));
	Aa->AttachToComponent(MeshComponent,FAttachmentTransformRules::KeepWorldTransform);
	Aa->ConstraintActor1 = this;
	//Aa->ComponentName1 = *MeshComponent->GetName();
	Aa->ConstraintActor2 = MainCharacter;
	Aa->SetAngularTwistLimit(ACM_Locked,0.f);
	Aa->SetAngularSwing1Limit(ACM_Locked,0);
	Aa->SetAngularSwing2Limit(ACM_Locked,0);
	Aa->InitComponentConstraint();
	FinishAddComponent(Aa,true,GetTransform());*/
	//FinishAddComponent(A,false,FTransform::Identity);
	/*A->ConstraintActor2 = this;
	A->InitComponentConstraint();
	A->UpdateConstraintFrames();*/

	
}

FVector AActorInteractObject::GetPlayerBackSocketPosition()
{
	return MainPlayerState->BackSocketCurrentLocation;
}

FRotator AActorInteractObject::GetPlayerDirectionRotator()
{
	return MainPlayerState->CurrentDirectionNormal.Rotation();
}

UStaticMeshComponent* AActorInteractObject::GetPlayerBackSocketComponent()
{
	return MainPlayerState->BackSocket;
}

void AActorInteractObject::SetPlayerPossessedInteractObjectSystem()
{
	UE_LOG(LogTemp,Warning,TEXT("BackStorageAttached"));
	SetActorLocation(this->GetPlayerBackSocketPosition());
	SetActorRotation(this->GetPlayerDirectionRotator());
	PrimitiveComponent->SetSimulatePhysics(false);
	PrimitiveComponent -> SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	IgnoreCamera = true;
	AttachToComponent(this->GetPlayerBackSocketComponent(),FAttachmentTransformRules::KeepWorldTransform);
	MainPlayerState->PossessedSystem = CurrentInteractObjectSystem;
	MainPlayerState->PossessedSystem->IsPossessedByPlayer = true;
	MainPlayerState->PossessedSystem->PlayerBackStorage = this;
	MainPlayerState->PossessedSocketPanel = &SocketPanel;
	
}

void AActorInteractObject::MergeWithPlayerPossessedInteractObjectSystem()
{
	//CurrentInteractObjectSystem = MainPlayerState->PossessedSystem;
	//MainGameState->InteractObjectSystemAddInteractObject(CurrentInteractObjectSystem,this);//Test,Not really merged
	if(MainPlayerState->PossessedSocketPanel == NULL)
		return;
	CurrentSocketInfo = FindTargetSocket();
	if(CurrentSocketInfo.BeginMerge)
	{
		int Tmp_BackStorageIndex = 0;
		int Tmp_ConnectObject2Index = 0;
		UE_LOG(LogTemp,Warning,TEXT("BackStorageAttached"));
		PrimitiveComponent->SetSimulatePhysics(false);
		PrimitiveComponent -> SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
		IgnoreCamera = true;
		//SetActorLocation(this->GetPlayerBackSocketPosition() - MainPlayerState->CurrentDirectionNormal*20);
		//SetActorRotation(this->GetPlayerDirectionRotator());

		AttachToComponent(MainPlayerState->PossessedSystem->PlayerBackStorage->FindComponentByClass<USkeletalMeshComponent>()
			,FAttachmentTransformRules::SnapToTargetNotIncludingScale
			,MainPlayerState->PossessedSocketPanel->
			Panel[CurrentSocketInfo.SocketRowIndex][CurrentSocketInfo.SocketColIndex].SocketName);
	

		for(int i = 0;AActorInteractObjectInterface* I: MainPlayerState->PossessedSystem->TotalInteractObjects)
		{
			if(I->BehaviorStatus.Equals("EquippedBackStorage"))
				Tmp_BackStorageIndex = i;
			i++;
		}
		for(int i = 0;AActorInteractObjectInterface* I: CurrentInteractObjectSystem->TotalInteractObjects)
		{
			if(I == this)
				Tmp_ConnectObject2Index = i;
			i++;
		}	
		MainGameState->MergeInteractObjectSystem(MainPlayerState->PossessedSystem,CurrentInteractObjectSystem
			,Tmp_BackStorageIndex,Tmp_ConnectObject2Index);
	}
	SetSocketsOccupied(CurrentSocketInfo.SocketRowIndex,CurrentSocketInfo.SocketColIndex);
}

void AActorInteractObject::SeperateFromPlayerPossessedInteractObjectSystem()
{
	if(Plug.CurrentSocket == NULL)
		return;
	int Tmp_BackStorageIndex = 0;
	int Tmp_DisConnectObject2Index = 0;
	UE_LOG(LogTemp,Warning,TEXT("DeattachedFromBackStorage"));
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PrimitiveComponent->SetSimulatePhysics(true);
	IgnoreCamera = false;
	for(int i = 0;AActorInteractObjectInterface* I: CurrentInteractObjectSystem->TotalInteractObjects)
	{
		if(I->BehaviorStatus.Equals("EquippedBackStorage"))
			Tmp_BackStorageIndex = i;
		i++;
	}
	for(int i = 0;AActorInteractObjectInterface* I: CurrentInteractObjectSystem->TotalInteractObjects)
	{
		if(I == this)
			Tmp_DisConnectObject2Index = i;
		i++;
	}	
	MainGameState->SeperateInteractObjectSystem(CurrentInteractObjectSystem
		,Tmp_BackStorageIndex,Tmp_DisConnectObject2Index);
	DisconnectFromSocket(CurrentSocketInfo.SocketRowIndex,CurrentSocketInfo.SocketColIndex);
	CurrentSocketInfo = TargetSocketInfo();
}

void AActorInteractObject::SetNewInteractObjectSystem()
{
	CurrentInteractObjectSystem = CurrentInteractObjectSystem -> NewInteractObjectSystem;
}

void AActorInteractObject::InitSocketPanel()
{
	int tmp_ColIndex = 0;
	int tmp_RowIndex = 0;
	int tmp_MaxColIndex = 0;
	int tmp_MaxRowIndex = 0;
	FString Stmp_ColIndex;
	FString Stmp_RowIndex;
	Socket tmp_NewSocket = Socket();
	TArray<FName> tmp_SocketNames =  MeshComponent->GetAllSocketNames();
	if (tmp_SocketNames.Num() != 0)
	{
		for (FName I : tmp_SocketNames)
		{
			FString tmp_I = I.ToString();
			if (tmp_I.RemoveFromStart("Socket"))
			{
				UE_LOG(LogTemp,Warning,TEXT("%s"),*tmp_I);
				tmp_I.Split(",",&Stmp_RowIndex,&Stmp_ColIndex);
				UE_LOG(LogTemp,Warning,TEXT("%s"),*Stmp_RowIndex);
				UE_LOG(LogTemp,Warning,TEXT("%s"),*Stmp_ColIndex);
				tmp_RowIndex = FCString::Atoi(*Stmp_RowIndex);
				tmp_ColIndex = FCString::Atoi(*Stmp_ColIndex);
				if (tmp_RowIndex > tmp_MaxRowIndex)
				{
					tmp_MaxRowIndex = tmp_RowIndex;
				}
				if (tmp_ColIndex > tmp_MaxColIndex)
				{
					tmp_MaxColIndex = tmp_ColIndex;
				}
			}
		}
		for (int i = 0; i<=tmp_MaxColIndex;i++)
		{
			SocketPanel.PanelRow.Add(tmp_NewSocket);
		}
		for (int i = 0; i<=tmp_MaxRowIndex;i++)
		{
			SocketPanel.Panel.Add(SocketPanel.PanelRow);
		}
		for (FName I : tmp_SocketNames)
		{
			FString tmp_I = I.ToString();
			if (tmp_I.RemoveFromStart("Socket"))
			{
				UE_LOG(LogTemp,Warning,TEXT("%s"),*tmp_I);
				tmp_I.Split(",",&Stmp_RowIndex,&Stmp_ColIndex);
				UE_LOG(LogTemp,Warning,TEXT("%s"),*Stmp_RowIndex);
				UE_LOG(LogTemp,Warning,TEXT("%s"),*Stmp_ColIndex);
				tmp_RowIndex = FCString::Atoi(*Stmp_RowIndex);
				tmp_ColIndex = FCString::Atoi(*Stmp_ColIndex);
				tmp_NewSocket.IsOccupied = false;
				tmp_NewSocket.SocketName = I;
				SocketPanel.Panel[tmp_RowIndex][tmp_ColIndex] = tmp_NewSocket;
			}
		}
	}
	for (TArray<Socket> I: SocketPanel.Panel)
		for (Socket G : I)
		{
			UE_LOG(LogTemp,Warning,TEXT("%s"),*G.SocketName.ToString());
		}

}

TargetSocketInfo AActorInteractObject::FindTargetSocket()
{
	TargetSocketInfo tmp_TargetSocketInfo = TargetSocketInfo();
	bool tmp_BeginMerge = false;
	bool tmp_OccupiedDetected = false;
	bool tmp_OutOfRange = false;
	int tmp_SocketRowIndex = 0;
	int tmp_SocketColIndex = 0;
	for(int i = 0;i<MainPlayerState->PossessedSocketPanel->Panel.Num();i++)
	{
		if(tmp_BeginMerge == true)
		{
			break;
		}
		for(int j = 0;j<MainPlayerState->PossessedSocketPanel->PanelRow.Num();j++)
		{
			for(int k = 0;k<=Plug.PlugRowMaxIndex;k++)
				for(int l = 0;l<=Plug.PlugColMaxIndex;l++)
				{
					if(i+k >= MainPlayerState->PossessedSocketPanel->Panel.Num()||
						j+l >= MainPlayerState->PossessedSocketPanel->PanelRow.Num())
						tmp_OutOfRange = true;
					else if(MainPlayerState->PossessedSocketPanel->Panel[i+k][j+l].IsOccupied)
					{
						tmp_OccupiedDetected = true;
					}
				}
			if (tmp_OccupiedDetected == false && tmp_OutOfRange == false)
			{
				tmp_SocketRowIndex = i;
				tmp_SocketColIndex = j;
				tmp_BeginMerge = true;
				break;
			}
			tmp_OccupiedDetected = false;
			tmp_OutOfRange = false;
		}
	}
	tmp_TargetSocketInfo.BeginMerge = tmp_BeginMerge;
	tmp_TargetSocketInfo.SocketRowIndex = tmp_SocketRowIndex;
	tmp_TargetSocketInfo.SocketColIndex = tmp_SocketColIndex;
	return tmp_TargetSocketInfo;
}

void AActorInteractObject::SetSocketsOccupied(int RowIndex,int ColIndex)
{
	for(int k = 0;k<=Plug.PlugRowMaxIndex;k++)
		for(int l = 0;l<=Plug.PlugColMaxIndex;l++)
		{
			MainPlayerState->PossessedSocketPanel->
				Panel[RowIndex+k][ColIndex+l].IsOccupied = true;
		}
	Plug.CurrentSocket = &MainPlayerState->PossessedSocketPanel->
		Panel[RowIndex][ColIndex];
}

void AActorInteractObject::DisconnectFromSocket(int RowIndex,int ColIndex)
{
	
	for(int k = 0;k<=Plug.PlugRowMaxIndex;k++)
		for(int l = 0;l<=Plug.PlugColMaxIndex;l++)
		{
			MainPlayerState->PossessedSocketPanel->
				Panel[RowIndex+k][ColIndex+l].IsOccupied = false;
		}
	Plug.CurrentSocket = NULL;
}

void AActorInteractObject::LogHello()
{
	UE_LOG(LogTemp,Warning,TEXT("Hello:)"));
}

//void AInteractObject::UpdateBehaviorStatus(FString NewItemStatus)
//{
//	Super::UpdateBehaviorStatus(NewItemStatus);
//}


void AActorInteractObject::OnCamBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IgnoreCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginOverLap CollisionObjectType: %d" ),OtherComp->GetCollisionObjectType());
		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PrimitiveComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Block);
	}
}

void AActorInteractObject::OnCamEndOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!IgnoreCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PrimitiveComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	}

}

// Called every frame
void AActorInteractObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//InteractObjectVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetRootComponent()->SetWorldRotation(MainPlayerState->CurrentDirectionNormal.Rotation());
	//Option1JustPressed = false;
	//Option2JustPressed = false;
	//Option3JustPressed = false;
	//Option4JustPressed = false;
}



// Called to bind functionality to input

