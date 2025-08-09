#pragma once
#include "SurviveGame/InteractObject/InteractObjectSocket.h"
struct Plug
{
	Socket* CurrentSocket;
	int PlugRowMaxIndex = 0;
	int PlugColMaxIndex = 0;
};
