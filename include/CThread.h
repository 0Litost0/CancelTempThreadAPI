/******************************************************************************


 ******************************************************************************
  文 件 名   : CThread.h
  版 本 号   : 初稿
  作    者   : Litost_Cheng
  生成日期   : 2018年11月30日
  最近修改   :
  功能描述   : CThread.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年11月30日
    作    者   : Litost_Cheng
    修改内容   : 创建文件

******************************************************************************/
#ifndef __CTHREAD_H__
#define __CTHREAD_H__

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
/*----------------------------------------------*
 * 宏定义                                 *
 *----------------------------------------------*/
///1： 使用信号取消线程序 非1： 使用pthread_cancel()取消线程
#define USE_SIGNAL 0


#define DEBUG_TEST(fmt,args...) printf("CThread:[%s][%d]:" fmt, __FILE__, __LINE__,##args)



/*----------------------------------------------*
 * 枚举声明                             *
 *----------------------------------------------*/
///<定义一个空类 用以获取成员函数指针，成员函数指针不能等同于普通函数指针
///详情见：http://www.dietmar-kuehl.de/mirror/c++-faq/pointers-to-members.html
class null_class
{
public:
        null_class(){}
        ~null_class(){}
};

typedef void * (*NormalFuncHandle)(void *);


typedef void * (null_class::*CallBack)(void *);


/*----------------------------------------------*
 * 结构体声明                             *
 *----------------------------------------------*/




/*----------------------------------------------*
 * 枚举定义                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 结构体定义                              *
 *----------------------------------------------*/
typedef struct tagClassCallBack
{
    null_class* pClass;
    CallBack pCallback;
}ClassCallBack_t;


typedef struct tagBind
{
    tagBind()
    {
        Reset();
    }
    void Reset()
    {
        m_ClassCallBack.pCallback = NULL;
        m_ClassCallBack.pClass = NULL;
        m_pNormalHandle = NULL;
        pArg = NULL;
    }


    ClassCallBack_t m_ClassCallBack;                //类成员函数回调
    NormalFuncHandle m_pNormalHandle;               //动作句柄
    void * pArg;
}Bind_t;

/*----------------------------------------------*
 * 外部函数原型说明                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                       *
 *----------------------------------------------*/


/**
 * @brief CThreadEvent class的简易说明
 *
 * 本范例说明CThreadEvent class。
 * 这是一个较为详细的范例。
 * 
 */
class CThreadEvent
{
    public:
        CThreadEvent():m_Pid(0),m_bThreadExit(true)
        {
            m_Bind.Reset();
            m_BindNext.Reset();
        }
        int Start(void *pClass, CallBack FuncHandle, void *pArg);
        int Start(NormalFuncHandle FuncHandle, void *pArg);
        int Cancel();
        void *_Start(void *pArg);
        bool IsRuning()
        {
           return !m_bThreadExit;
        }

    private:
        Bind_t m_Bind;
        Bind_t m_BindNext;
        pthread_t m_Pid;				///<线程id	考虑到同一个进程中，线程ID会复用，因此在进行线程标识判断时，要格外小心

        volatile bool m_bThreadExit;			///<线程是否已经退出
        ///pthread_attr_t m_Attr;			///<线程属性
        void * PthreadCleanUp(void *);
};



#endif /* __CTHREAD_H__ */
