#include "MainGameStateInterface.h"

InteractObjectSystem* AMainGameStateInterface::CreateInteractObjectSystem(AActorInteractObjectInterface* FirstInteractObject)
{
	InteractObjectSystem* Default = new InteractObjectSystem();
	return Default;
}

void AMainGameStateInterface::InteractObjectSystemAddInteractObject(InteractObjectSystem* TargetInteractObjectSystem,
	AActorInteractObjectInterface* NewInteractObject)
{
	
}

InteractObjectSystem* AMainGameStateInterface::MergeInteractObjectSystem(InteractObjectSystem* Source1,InteractObjectSystem* Source2,
	int ConnectObject1Index,int ConnectObject2Index)
{
	
	InteractObjectSystem* Default = new InteractObjectSystem();
	return Default;
}

InteractObjectSystem* AMainGameStateInterface::SeperateInteractObjectSystem(InteractObjectSystem* Source1,
		int DisConnectObject1Index,int DisConnectObject2Index)
{
	InteractObjectSystem* Default = new InteractObjectSystem();
	return  Default;
}

InteractObjectSystem* AMainGameStateInterface::SeperateInteractObjectSystem()
{
	InteractObjectSystem* Default = new InteractObjectSystem();
	return Default;
}

InteractObjectSystem* AMainGameStateInterface::DestroyInteractObjectSystem()
{
	InteractObjectSystem* Default = new InteractObjectSystem();
	return Default;
}

void AMainGameStateInterface::PrintConnectionMatrix(InteractObjectSystem)
{
}

