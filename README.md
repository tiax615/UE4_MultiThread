# UE4_MultiThread
UE4.24多线程

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

## 999. 参考资料
1. 官方Wiki，作者Rama https://www.ue4community.wiki/Legacy/Multi-Threading:_How_to_Create_Threads_in_UE4
2. 官方Wiki，作者Rama https://www.ue4community.wiki/Legacy/Multi-Threading:_Task_Graph_System