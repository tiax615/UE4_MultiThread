// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "PrimeNumberWorker.h"

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
