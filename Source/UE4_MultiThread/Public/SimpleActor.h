// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleActor.generated.h"

UCLASS()
class UE4_MULTITHREAD_API ASimpleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "SimpleActor")
		void RunSimpleRunnable();

	UFUNCTION(BlueprintCallable, Category = "SimpleActor")
		void StopSimpleRunnable();

	UFUNCTION(BlueprintCallable, Category = "SimpleActor")
		void StartTaskGraph(int TotalToGet);

	UFUNCTION(BlueprintCallable, Category = "SimpleActor")
		void StartAsyncTask();

private:
	void CheckAllTasksDone();

	FTimerHandle MyTimerHandle;
};

class FSimpleAsyncTasks : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FSimpleAsyncTasks>;
public:
	FSimpleAsyncTasks(int32 Input1, int32 Input2);

protected:
	int32 MyInput1;
	int32 MyInput2;
	void DoWork();
	FORCEINLINE TStatId GetStatId() const;
};