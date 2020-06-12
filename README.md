# UE4_MultiThread
UE4.24.3多线程

示例项目：https://github.com/tiax615/UE4_MultiThread

## 0. 背景
UE4大部分工作都是在主线程里完成的，但有些事情可以放在多线程里，所以需要了解多线程。和渲染以及UObject/AActor的创建/修改/销毁有关的事请，应该在主线程完成，否则会崩溃。其他线程可能可以做网络数据收发和解析，数学运算等。

研究过程中发现Rama大佬写的官方Wiki代码比较复杂，其中FRunnable可以运行，但TaskGraphSystem跑不动。也许使用UE4多线程，应从简单的入手，了解本质。

## 1. 项目结构
```
Content/                                            - 资源文件夹
    Rama/                                           - 官方Wiki尝试
        BP_MultiThreadGameMode
        BP_MyPlayerController
        BP_TestGameMode
        MultiThread
Source/                                             - cpp文件夹
    Private/
        PrimeNumberWorker.cpp                        - 纯cpp类继承FRunnable
        MyPlayerController.cpp                      - 命名空间中实现TaskGraph，以及以上FRunnable的使用
```

## 2. 官方Wiki
大佬的代码，我看着有点吃力。
### 2.1. FRunnable
详见PrimeNumberWorker.h/PrimeNumberWorker.cpp
### 2.2. TaskGraph
详见MyPlayerController.h/MyPlayerController.cpp

## 3. FRunnable
### 3.1. 最简单的形式
最简单的FRunnable线程实现代码如下，这个能运行，每隔0.5s打印当前Count的值。但是因为只实现了Run()，并且永远循环，所以线程无法退出。就算游戏主线程停止了，这个线程还在继续运行。

如果Run()中的方法不是永远循环的，就可以直接退出。

SimpleRunnable.h
```
#include "CoreMinimal.h"
#include "HAL/Runnable.h"

class FSimpleRunnable :public FRunnable
{
public:
	FSimpleRunnable();
	~FSimpleRunnable();

private:
	// 必须实现的几个
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
};
```

只实现了一个简单的Run()，一直循环每隔0.5s打印Count的值，并且Count++。头文件PlayerController.h是为了使用FPlatformProcess::Sleep()。

SimpleRunnable.cpp
```
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
```

以上实现了一个FRunnable，但还没有去调用，需要在主线程调用。这里新建了一个Actor类SimpleActor，在新增的RunSimpleRunnable()中调用。需要包含HAL/RunnableThread.h。

SimpleActor.cpp
```
#include "SimpleActor.h"
#include "HAL/RunnableThread.h"
#include "SimpleRunnable.h"

//...

void ASimpleActor::RunSimpleRunnable()
{
	FRunnable* SimpleRunnable = new FSimpleRunnable();
	FRunnableThread* SimpleRunnableThread = FRunnableThread::Create(SimpleRunnable, TEXT("MySimpleRunnable"));
}
```

使用的时候出现一个奇怪的现象，每次打印的不是连续的整数，而是每个数都多加了1

![](./MultiThread/MultiThread1.png)

## 999. 参考资料
1. 官方Wiki，作者Rama https://www.ue4community.wiki/Legacy/Multi-Threading:_How_to_Create_Threads_in_UE4
2. 官方Wiki，作者Rama https://www.ue4community.wiki/Legacy/Multi-Threading:_Task_Graph_System