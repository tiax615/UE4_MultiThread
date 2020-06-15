#include "SimpleActor.h"
#include "SimpleRunnable.h"

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