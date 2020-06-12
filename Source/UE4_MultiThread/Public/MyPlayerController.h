 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE4_MULTITHREAD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MultiThread")
	TArray<uint32> PrimeNumbers;

	// 定时器句柄
	FTimerHandle MyTimerHand;

	// 开启FRunnable多线程
	UFUNCTION(BlueprintCallable, Category = "MultiThread")
		void RunMultiThread();

	// 关闭FRunnable多线程，不关会炸
	UFUNCTION(BlueprintCallable, Category = "MultiThread")
		void StopMultiThread();

	// int和size_t大小比较
	UFUNCTION(BlueprintCallable, Category = "cao")
		bool Cao();

	// 开启TaskGraphSysten
	UFUNCTION(BlueprintCallable, Category = "MultiThread")
		void StartTask();

	// 检查是否所有Task结束了
	UFUNCTION(BlueprintCallable, Category = "MultiThread")
		void VictoryCheckAllThreadsDone();
};
