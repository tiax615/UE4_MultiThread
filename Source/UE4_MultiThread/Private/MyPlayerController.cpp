// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "PrimeNumberWorker.h"
//#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Multi thread Test, finding prime number
namespace VictoryMultiThreadTest
{
	// Multi-thread link to UObjects, do not create/modify/destroy UObjects/AActors via this link!

	APlayerController* ThePC;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// OUTPUT RESULTS OF TASk THREADS
	TArray<uint32> PrimeNumbers;
	// This is the array of thread completions to know if all threads are done yet
	FGraphEventArray VictoryMultiThreadTest_CompletionEvents;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Are ALL Tasks Complete?
	// 检查是不是所有任务都完成了
	bool TasksAreComplete()
	{
		// Check all thread completion events
		for (int32 Index = 0; Index < VictoryMultiThreadTest_CompletionEvents.Num(); Index++)
		{
			// If ! IsComplete()
			if (!VictoryMultiThreadTest_CompletionEvents[Index]->IsComplete())
			{
				return false;
			}
		}
		return true;
	}

	// Actual Task Code
	int32 FindNextPrimeNumber()
	{
		// Last known prime number +1
		int32 TestPrime = PrimeNumbers.Last();

		bool NumIsPrime = false;
		while (!NumIsPrime)
		{
			NumIsPrime = true;

			// Try Next Number
			TestPrime++;

			// Modulus from 2 to current number -1
			for (int32 b = 2; b < TestPrime; b++)
			{
				if (TestPrime%b == 0)
				{
					NumIsPrime = false;
					break;
				}
			}
		}

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Did another thread find this number already?
		if (PrimeNumbers.Contains(TestPrime))
		{
			return FindNextPrimeNumber(); // recursion 递归
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// Success!
		return TestPrime;
	}

	// Each Task Thread
	class FVictoryTestTask
	{
	public:
		FVictoryTestTask() // send in property defaults here
		{
			// can add properties here
		}

		// return the name of the task
		static const TCHAR* GetTaskName()
		{
			return TEXT("FVictoryTestTask");
		}
		FORCEINLINE static TStatId GetStatId() // 干啥的???
		{
			RETURN_QUICK_DECLARE_CYCLE_STAT(FVictoryTestTask, STATGROUP_TaskGraphTasks);
		}
		// return the thread for this task
		static ENamedThreads::Type GetDesiredThread()
		{
			return ENamedThreads::AnyThread;
		}

		// 干啥的???
		static ESubsequentsMode::Type GetSubsequentsMode()
		{
			return ESubsequentsMode::TrackSubsequents;
		}

		// Main Function: Do Task!
		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			PrimeNumbers.Add(FindNextPrimeNumber());

			/************************************************************************/
			/*
				Show Incremental Results in Main Game Thread!
				Please note you should not create/destory/modify UObjects here
				Do those sort of things after all thread are completed

				All calcs for making stuff can be done in the threads
				But the actual making/modify of the UObjects should be done in main game thread
				which is AFTER all tasks have completed :)))))))
			*/
			ThePC->ClientMessage(FString("A thread completed! ~") + FString::FromInt(PrimeNumbers.Last()));
			/************************************************************************/
		}
	};

	// Multi-Task Initiation Point
	void FindPrimes(const uint32 TotalToFind)
	{
		PrimeNumbers.Empty();
		PrimeNumbers.Add(2);
		PrimeNumbers.Add(3);

		// Add thread/task for each of total prime numbers to find
		for (uint32 b = 0; b < TotalToFind; b++)
		{
			// add properties inside ConstructAndDispatchWhenReady()
			VictoryMultiThreadTest_CompletionEvents.Add(TGraphTask<FVictoryTestTask>::CreateTask(
				NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady());
			//FPlatformProcess::Sleep(0.01);
		}
	}
}

void AMyPlayerController::RunMultiThread()
{
	// Multi-threading, returns handle that could be cached
	// use static function FPrimeNumberWorker::Shutdown() if necessary
	FPrimeNumberWorker::JoyInit(PrimeNumbers, 50000, this);
}

void AMyPlayerController::StopMultiThread()
{
	FPrimeNumberWorker::Shutdown();
}

bool AMyPlayerController::Cao()
{
	size_t t = 10086;
	int a = -1;
	if (t > a)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Starting the Tasks/Threads
void AMyPlayerController::StartTask()
{
	VictoryMultiThreadTest::ThePC = this;
	VictoryMultiThreadTest::FindPrimes(50000); // first 50000 prime numbers

	// Start a timer to check when all the threads are done!
	// Rama定时器的写法可能有问题
	//GetWorldTimerManager().SetTimer(this, &AMyPlayerController::VictoryCheckAllThreadsDone, 1, true);
	GetWorldTimerManager().SetTimer(MyTimerHand, this, &AMyPlayerController::VictoryCheckAllThreadsDone, 1, true);
}

// timer to check when thread are done
// Please note timers must be in the game thread/main/normal thread
void AMyPlayerController::VictoryCheckAllThreadsDone()
{
	if (VictoryMultiThreadTest::TasksAreComplete())
	{
		// Clear Timer
		// Rama定时器的写法可能有问题
		//GetWorldTimerManager().ClearTimer(this, &APlayerController::VictoryCheckAllThreadsDone);
		GetWorldTimerManager().ClearTimer(MyTimerHand);

		ClientMessage("Multi Thread Test Done!");
		//UE_LOG(LogTemp, Warning, TEXT("Prime Numbers Found:"));
		//for (int32 v = 0; v < VictoryMultiThreadTest::PrimeNumbers.Num(); v++)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("%d~%d"), v, VictoryMultiThreadTest::PrimeNumbers[v]);
		//}
	}
}
