#include "SimpleRunnable.h"
#include "GameFramework\PlayerController.h"
//#include "Windows\WindowsPlatformProcess.h"

FSimpleRunnable::FSimpleRunnable() {}

FSimpleRunnable::~FSimpleRunnable() {}

bool FSimpleRunnable::Init() { return true; }

uint32 FSimpleRunnable::Run()
{
	int Count = 0;
	while (true)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), Count++);
		FPlatformProcess::Sleep(0.5);
	}
	return 0;
}

void FSimpleRunnable::Stop() {}

void FSimpleRunnable::Exit() {}