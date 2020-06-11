 

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

	UFUNCTION(BlueprintCallable, Category = "MultiThread")
		void RunMultiThread();

	UFUNCTION(BlueprintCallable, Category = "MultiThread")
		void StopMultiThread();
};
