#include "SimpleActor.h"
#include "SimpleRunnable.h"
#include "TimerManager.h"

namespace SimpleTaskGraph
{
	FGraphEventArray SimpleTask_CompletionEvents; // 用于保存任务的数组
	int Number = 0; // 要打印的整数，从0开始

	// Are all tasks complete?
	bool TasksAreComplete()
	{
		for (int32 Index = 0; Index < SimpleTask_CompletionEvents.Num(); Index++)
		{
			if (!SimpleTask_CompletionEvents[Index]->IsComplete())
			{
				return false;
			}
		}
		return true;
	}

	// 打印下一个整数
	int GetNextInt()
	{
		FPlatformProcess::Sleep(0.1);
		Number++;
		UE_LOG(LogTemp, Warning, TEXT("SimpleTask %d"), Number);
		return Number;
	}

	// Each task thread
	class SimpleTask
	{
	public:
		SimpleTask() {}

		// 获取任务的名字，暂时没用到
		static const TCHAR* GetTaskName()
		{
			return TEXT("SimpleTask");
		}

		//////////////////////////////////////////////////////////////////////////
		// 需要实现的方法，没有这些会编译报错。暂时没用到
		FORCEINLINE static TStatId GetStatId()
		{
			RETURN_QUICK_DECLARE_CYCLE_STAT(SimpleTask, STATGROUP_TaskGraphTasks);
		}
		static ESubsequentsMode::Type GetSubsequentsMode()
		{
			return ESubsequentsMode::TrackSubsequents;
		}
		static ENamedThreads::Type GetDesiredThread()
		{
			return ENamedThreads::AnyThread;
		}
		//////////////////////////////////////////////////////////////////////////

		// Main function: DO Task!
		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			GetNextInt();
		}
	};

	// 使用命名空间SimpleTaskGraph的这个方法，开启任务图
	void GetInts(const uint32 TotalToGet)
	{
		for (uint32 i = 0; i < TotalToGet; i++)
		{
			// 第一个变量等待其中的任务完成，然后再进行下一个任务
			// 第一个变量官方Wiki是NULL，这样可以不按照顺序执行
			SimpleTask_CompletionEvents.Add(TGraphTask<SimpleTask>::CreateTask(
				&SimpleTask_CompletionEvents, ENamedThreads::GameThread).ConstructAndDispatchWhenReady());
		}
	}
}

// Sets default values
ASimpleActor::ASimpleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimpleActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASimpleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASimpleActor::RunSimpleRunnable()
{
	FSimpleRunnable::JoyInit();
}

void ASimpleActor::StopSimpleRunnable()
{
	FSimpleRunnable::Shutdown();
}

void ASimpleActor::StartTask(int TotalToGet)
{
	SimpleTaskGraph::GetInts(TotalToGet);
	GetWorldTimerManager().SetTimer(MyTimerHandle, this, &ASimpleActor::CheckAllTasksDone, 1, true);
}

void ASimpleActor::CheckAllTasksDone()
{
	if (SimpleTaskGraph::TasksAreComplete())
	{
		GetWorldTimerManager().ClearTimer(MyTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("TaskGraph Done!"));
	}
}