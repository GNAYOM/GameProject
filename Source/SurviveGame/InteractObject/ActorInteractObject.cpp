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
	UE_LOG(LogTemp,Warning,TEXT("%d"),CurrentInteractObjectSystem->ConnectionMatrix[0][0]);
	UE_LOG(LogTemp,Warning,TEXT("%d"),CurrentInteractObjectSystem->TotalInteractObjects.Num());
	CurrentInteractObjectSystem->TotalInteractObjects[CurrentInteractObjectSystem->TotalInteractObjects.Find(this)]->LogHello();
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
	UE_LOG(LogTemp, Warning, TEXT("BeginOverLap CollisionObjectType: %d" ),OtherComp->GetCollisionObjectType());
	
	PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimitiveComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Block);
	
}

void AActorInteractObject::OnCamEndOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
	PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimitiveComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
}

// Called every frame
void AActorInteractObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GetRootComponent()->SetWorldRotation(MainPlayerState->CurrentDirectionNormal.Rotation());
	//Option1JustPressed = false;
	//Option2JustPressed = false;
	//Option3JustPressed = false;
	//Option4JustPressed = false;
}



// Called to bind functionality to input

