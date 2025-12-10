// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractManagerComponent.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractManagerComponent::UInteractManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	RootSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("RootSkeletalMesh");
	InteractManagerComponentStatus = InteractManagerComponentStatusEnum::SelectByRange;
	// ...
}


// Called when the game starts
void UInteractManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	RootSkeletalMesh = GetAttachmentRootActor()->GetComponentByClass<USkeletalMeshComponent>();
	InteractObjectDetector = GetAttachmentRootActor()->GetComponentByClass<UBoxComponent>();
	if(InteractObjectDetector)
	{
		UE_LOG(LogTemp,Warning,TEXT("Complete"));
		InteractObjectDetector->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractObjectDetector->OnComponentBeginOverlap.AddDynamic(this,&UInteractManagerComponent::OnInteractObjectBeginOverlap);
		InteractObjectDetector->OnComponentEndOverlap.AddDynamic(this,&UInteractManagerComponent::OnInteractObjectEndOverlapEnd);
	}
	MainPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMainPlayerState>();	
	// ...
	
}


void UInteractManagerComponent::InteractObjectSelection()
{
	if(InteractManagerComponentStatus == SelectByRange)
	{
		SelectedTargetInteractObject = SelectInteractObjectByRange();
	}
	else if (InteractManagerComponentStatus == SelectFromPlayerBackSocket)
	{
		SelectedTargetInteractObject = SelectInteractObjectFromBackSocket();
	}
	else if(InteractManagerComponentStatus == BlockingUseEquipment)
	{
		SelectedTargetInteractObject = EquippedInteractObject;
		SelectedTargetInteractObject->TargetInteractObjectByRange = SelectInteractObjectByRange();
	}
	if(SelectedTargetInteractObject != NULL)
		SelectedTargetInteractObject->IsSelectedByPlayer = true;//本次选中的物体被玩家选中的标志位设置为true
	if(LastSelectedTargetInteractObject != NULL
		&& LastSelectedTargetInteractObject != SelectedTargetInteractObject)//如果上一个被选中的物体不为空且上一个选中的物体不等于当前选中的物体
	{
		LastSelectedTargetInteractObject->IsSelectedByPlayer = false;//将上一个物体被玩家选中的标志置零
	}
	LastSelectedTargetInteractObject = SelectedTargetInteractObject;//将上一个选中的物体设置为本次选中的物体

}

AActorInteractObjectInterface* UInteractManagerComponent::SelectInteractObjectByRange()
{
	
	tmp_InteractObjectsSorted = InRangeInteractObjects;
	if(tmp_InteractObjectsSorted.Contains(EquippedInteractObject))
		tmp_InteractObjectsSorted.Remove(EquippedInteractObject);//排序不包括正在被装备的
	float tmp_CurrentDistanceA = 0.f;
	float tmp_CurrentDistanceB = 0.f;
	AActorInteractObjectInterface* tmp_PosSwap = NULL;
	//In range InteractObject Sort by distance
	for(int i = 0; i < tmp_InteractObjectsSorted.Num(); i++)
	{
		for(int j = i;j<tmp_InteractObjectsSorted.Num();j++)
		{
			tmp_CurrentDistanceA = (RootSkeletalMesh -> GetComponentLocation()
				- tmp_InteractObjectsSorted[i] -> GetComponentByClass<UBoxComponent>() -> GetComponentLocation()).Length();
			tmp_CurrentDistanceB = (RootSkeletalMesh -> GetComponentLocation()
				- tmp_InteractObjectsSorted[j] -> GetComponentByClass<UBoxComponent>() -> GetComponentLocation()).Length();
			if(tmp_CurrentDistanceA > tmp_CurrentDistanceB)
			{
				tmp_PosSwap = tmp_InteractObjectsSorted[j];
				tmp_InteractObjectsSorted[j] = tmp_InteractObjectsSorted[i];
				tmp_InteractObjectsSorted[i] = tmp_PosSwap;
			}
		}
	}
	for(int i =0;AActorInteractObjectInterface* IO : tmp_InteractObjectsSorted)
	{
		
		FString IOName = IO -> BehaviorStatus;
		//UE_LOG(LogTemp,Warning,TEXT("%d  %s"),i,*IOName);
		i++;
	}
	InteractObjectsSorted = tmp_InteractObjectsSorted;
	if(!InteractObjectsSorted.IsEmpty())//如果排序队列不为空
	{
		return InteractObjectsSorted[0];//选中最近的物体
	}
	else
	{
		return  NULL;
	}

	// ...
}

AActorInteractObjectInterface* UInteractManagerComponent::SelectInteractObjectFromBackSocket()
{
	if(MainPlayerState->PossessedSystem->TotalInteractObjects.Num() > 1)
	{
		for(AActorInteractObjectInterface* I : MainPlayerState->PossessedSystem->TotalInteractObjects)
			if (I->BehaviorStatus != "EquippedBackStorage")
			{
				return I;
			}
		return NULL;
	}
	return NULL;
		//MainPlayerState->InteractManagerComponentStatus = SelectByRange;
}

// Called every frame
void UInteractManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	InteractObjectSelection();
}

void UInteractManagerComponent::InteractObjectInputDetected(int Input)
{
	
	if (InteractManagerComponentStatus == BlockingUseEquipment)
	{
		EquippedInteractObject ->ReleaseEventActively(Input);
	}
	else
	{
		if(SelectedTargetInteractObject != NULL)
		{
			SelectedTargetInteractObject->ReleaseEventActively(Input);
			//GetAttachmentRootActor()->GetComponentByClass<UPhysicsConstraintComponent>()->ConstraintActor2 = InteractObjectsSorted[0];
		
		}
	}
	

}

void UInteractManagerComponent::OnInteractObjectBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	FString IOName = OtherActor->GetName();
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap:%s"), *IOName);
	AActorInteractObjectInterface* InsertNewInteractObject =  Cast<AActorInteractObjectInterface>(OtherActor);
	InRangeInteractObjects.Add(InsertNewInteractObject);
	//InsertNewInteractObject->ReleaseEventActive(1);
}

void UInteractManagerComponent::OnInteractObjectEndOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FString IOName = OtherActor->GetName();
	UE_LOG(LogTemp,Warning,TEXT("EndOverlap111:%s"),*IOName);
	AActorInteractObjectInterface* InsertNewInteractObject =  Cast<AActorInteractObjectInterface>(OtherActor);
	InRangeInteractObjects.Remove(InsertNewInteractObject);
}

