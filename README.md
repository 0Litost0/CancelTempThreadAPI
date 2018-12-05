@[TOC](CancelTempThreadAPI)
# CancelTempThreadAPI
## 一、摘要
&emsp;&emsp;This repository provide a set of API (Packaged into classes using C++) :could Create and cancel the tempory thread。  

&emsp;&emsp;在实际工作过程中，有时需要我们对一些临时性的工作进行处理，传统的处理方式可能是：在一个工作线程中不断轮训相应事件标志，置位则执行，反之则取消。但是对于一些连贯性动作，或是在调用第三方接口时，可能无法适时终止相应执行流。为了解决以上问题，该仓库提供了一组API，通过调用相关对象，可以创建或是取消一个临时线程。

