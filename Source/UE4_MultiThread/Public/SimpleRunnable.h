#include "CoreMinimal.h"
#include "HAL/Runnable.h"

class FSimpleRunnable :public FRunnable
{
public:
	FSimpleRunnable();
	~FSimpleRunnable();

	static FSimpleRunnable* MySimpleRunnable;
	static FSimpleRunnable* JoyInit();

	static void Shutdown();

private:
	// 必须实现的几个
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	class FRunnableThread* MyRunnableThread;
	FThreadSafeCounter StopTaskCounter;
	void EnsureCompletion();
};