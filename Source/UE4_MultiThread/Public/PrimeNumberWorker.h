// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL\Runnable.h"

/**
 * https://www.ue4community.wiki/Legacy/Multi-Threading:_How_to_Create_Threads_in_UE4
 * Multi Threading
 */
//class UE4_MULTITHREAD_API PrimeNumberWorker
class FPrimeNumberWorker : public FRunnable
{
	// Singleton实例，可通过StaticAccessor在任何时候访问该线程，如果是Active的
	static FPrimeNumberWorker* Runnable;

	// Thread to run the worker FRunnable on
	FRunnableThread* Thread;

	// The Data Ptr
	TArray<uint32>* PrimeNumbers;

	// The PC
	class APlayerController* ThePC;

	// Stop this thread? Uses Thread Safe Counter
	FThreadSafeCounter StopTaskCounter;

	// The actual finding of prime numbers，素数
	int32 FindNextPrimeNumber();

private:
	int32 PrimesFoundCount;

public:
	int32 TotalPrimesToFind;

	// Done?
	bool IsFinished() const
	{
		return PrimesFoundCount >= TotalPrimesToFind;
	}

	// ~~~~~~Thread Core Functions~~~~~~

	// Constructor/Destructor
	FPrimeNumberWorker(TArray<uint32>& TheArray, const int32 IN_TotalPrimesToFind, class APlayerController* IN_PC);
	virtual ~FPrimeNumberWorker();

	// Begin FRunable interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunable interface

	// Make sure this thread has stopped properly，确保正常停止
	void EnsureCompletion();

	//~~~~~~Starting and Stopping Thread~~~~~~

	/*
		Start the thread and the worker from static (easy access)!
		This code ensures only 1 Prime Number thread will be able to run at a time
		This function returns a handle to the newly started instance
	*/
	static FPrimeNumberWorker* JoyInit(TArray<uint32>& TheArray, const int32 IN_TotalPrimesToFind, class APlayerController* IN_PC);

	// Shuts down the thread. Static so it can easily be called from outside the thread context
	static void Shutdown();
	static bool IsThreadFinished();
};
