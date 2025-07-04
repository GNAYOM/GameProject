#include "MainGameStateInterface.h"

InteractObjectSystem* AMainGameStateInterface::CreateInteractObjectSystem(AActorInteractObjectInterface* FirstInteractObject)
{
	InteractObjectSystem* Default = new InteractObjectSystem();
	return Default;
}

InteractObjectSystem* AMainGameStateInterface::MergeInteractObjectSystem()
{
	
	InteractObjectSystem* Default = new InteractObjectSystem();
	return Default;
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

