#pragma once

struct Socket
{
	FVector WorldLocation;
	FName SocketName;
	bool IsOccupied;
};

struct SocketPanel
{
	TArray<Socket> PanelRow;
	TArray<TArray<Socket>> Panel;
};

