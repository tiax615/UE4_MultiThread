#include "SimpleRunnable.h"
#include "HAL/RunnableThread.h"
#include "GameFramework\PlayerController.h"
//#include "Windows\WindowsPlatformProcess.h"

FSimpleRunnable* FSimpleRunnable::MySimpleRunnable = nullptr;

FSimpleRunnable::FSimpleRunnable():StopTaskCounter(0)
{
	MyRunnableThread = FRunnableThread::Create(this, TEXT("MySimpleRunnable"));
}

FSimpleRunnable::~FSimpleRunnable()
{
	delete MyRunnableThread;
	MyRunnableThread = nullptr;
}

FSimpleRunnable* FSimpleRunnable::JoyInit()
{
	if (!MySimpleRunnable && FPlatformProcess::SupportsMultithreading())
	{
		MySimpleRunnable = new FSimpleRunnable();
	}
	return MySimpleRunnable;
}

void FSimpleRunnable::EnsureCompletion()
{
	Stop();
	MyRunnableThread->WaitForCompletion();
}

void FSimpleRunnable::Shutdown()
{
	if (MySimpleRunnable)
	{
		MySimpleRunnable->EnsureCompletion();
		delete MySimpleRunnable;
		MySimpleRunnable = nullptr;
	}
}

bool FSimpleRunnable::Init()
{ 
	UE_LOG(LogTemp, Warning, TEXT("SimpleRunnable Init"));
	return true;
}

uint32 FSimpleRunnable::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("SimpleRunnable Run"));

	int Count = 0;
	while (StopTaskCounter.GetValue()==0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), Count);
		Count++;
		FPlatformProcess::Sleep(0.5);
	}
	return 0;
}

void FSimpleRunnable::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("SimpleRunnable Stop"));
	StopTaskCounter.Increment();
}

void FSimpleRunnable::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("SimpleRunnable Exit"));
}

