* [CancelTempThreadAPI](#CancelTempThreadAPI)
* [一、简介](#一、简介)
* [二、快速上手](#二、快速上手)
    * [1. 依赖环境](#1.依赖环境)
    * [2.对象调用](#2.对象调用)
    * [3.线程创建](#3.线程创建)
    * [4.线程取消](#4.线程取消)
* [三、实现原理](#三、实现原理)
    * [1.执行流执行完毕，正常退出](#1.执行流执行完毕，正常退出)
    * [2.其它线程主动取消临时线程](#2.其它线程主动取消临时线程)
* [四、参考与链接](#四、参考与链接)
<h1 id="CancelTempThreadAPI">CancelTempThreadAPI</h1>
<h2 id="一、简介">一、简介</h2>
&emsp;&emsp;Provide a set of API (Packaged into classes using C++) :could create and cancel the tempory thread。  

&emsp;&emsp;`在实际工作过程中，有时需要我们对一些临时性的工作进行处理，传统的处理方式可能是：在一个工作线程中不断轮训相应时间标志，置位则执行，反之则取消。但是对于一些连贯性动作，或是在调用第三方接口时，可能无法适时终止相应执行流。为了解决以上问题，该仓库封装了一个C++类，通过调用相关对象，可以创建或是取消一个临时线程。`  

***
<h2 id="二、快速上手">二、快速上手</h2>

<h3 id="1.依赖环境">1. 依赖环境</h3>
&emsp;&emsp;使用相应接口，需要包含以下头文件  

```c++
	#include <CThread.h>
```
<h3 id="2.对象调用">2.对象调用</h3>

```c++
	CThreadEvent TempThread;
```
<h3 id="3.线程创建">3.线程创建</h3>

&emsp;&emsp;用户可以调用方以下两个接口，来创建线程：

```c++
	int CThreadEvent::Start(void *pClass , CallBack FuncHandle, void * pArg);	///通过该接口在新建线程中执行常规的成员函数
```
```c++
	int CThreadEvent::Start(NormalFuncHandle FuncHandle, void *pArg);		///通过该接口在新建线程中执行常规函数
```
<h3 id="4.线程取消">4.线程取消</h3>

```c++
	int CThreadEvent::Cancel();	///通过该接口以取消临时线程的执行
```
***
<h2 id="三、实现原理"> 三、实现原理</h2>
&emsp;&emsp;此套接口通过创建临时线程的方式，处理用户指定执行流。根据临时线程终止原因的不同，可以分为以下几种情况：  
<h3 id="1.执行流执行完毕，正常退出">1.执行流执行完毕，正常退出</h3>


![临时线程执行流正常终止](/doc/NormalExitTempThread.png "临时线程执行流正常终止")

<h3 id="2.其它线程主动取消临时线程">2.其它线程主动取消临时线程</h3>

![主动终止线程](/doc/TerminateTempThread.png "主动终止线程")

&emsp;&emsp;关于cancel的实现，主要存在以下两种方式：  
1. pthread_cancel()  
&emsp;&emsp;此种方式要求在创建临时线程时，需要[设置线程的分离属性][]。并调用pthread_cancel()来终止线程。  
2. pthread_kill()  
&emsp;&emsp;此种要是要求用户在创建临时线程时，必须先为指定信号设置信号处理函数，临时线程在收到相应线程后，终止本线程执行。

*注：关于线程与信号的相关知识参见[《UNIX环境高级编程》][]*
>![《UNIX环境高级编程》](/doc/ThreadAndSignal.png "——《UNIX环境高级编程》")  

&emsp;&emsp;此外，在取消临时线程时，我们还不得不考虑资源回收的问题，具体的情境如下图所示:
![主动终止线程](/doc/MemoryLeak.png "主动终止线程")  
&emsp;&emsp;倘若我们在指定的执行流ProcessaA中申请了堆栈、互斥锁、信号量等资源，那么在临时线程被意外终止时，会导致相应的资源没有释放(即使我们在线程退出前执行了线程清理函数)，从而造成内存泄漏。这也是为什么在安卓中没有实现[pthread_cancel()的原因][android bionic缺失pthread_cancel的解决方法]。

*注：该接口包含上述两种实现。通过`USE_SIGNAL`宏来决定具体使用何种方式*
```c++
    ///1： 使用信号取消线程序 非1： 使用pthread_cancel()取消线程
    #define USE_SIGNAL 0
```
***
<h2 id="四、参考与链接"> 四、参考与链接</h2>
1. 《UNIX环境高级编程》:https://zh.wikipedia.org/wiki/UNIX%E7%92%B0%E5%A2%83%E9%AB%98%E7%B4%9A%E7%B7%A8%E7%A8%8B  
2. 设置线程的分离属性:http://www.cnblogs.com/lijunamneg/archive/2013/01/25/2877211.html  
3. android bionic缺失pthread_cancel的解决方法:https://blog.csdn.net/yzx41099296/article/details/7692484

[《UNIX环境高级编程》]:https://zh.wikipedia.org/wiki/UNIX%E7%92%B0%E5%A2%83%E9%AB%98%E7%B4%9A%E7%B7%A8%E7%A8%8B  
[设置线程的分离属性]: http://www.cnblogs.com/lijunamneg/archive/2013/01/25/2877211.html  
[android bionic缺失pthread_cancel的解决方法]:https://blog.csdn.net/yzx41099296/article/details/7692484





