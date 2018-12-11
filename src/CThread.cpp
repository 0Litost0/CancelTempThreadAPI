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
#include <CThread.h>

/**
*@brief 返回值为(void *)的函数（普通函数与成员函数）入口点
*
* FuncHandleEntryRtnVoidPtr我是一个详细的简介，紧接brief，且没有换行
*@param void * pArg  
*@return static
*@author yccheng
 *日    期   : 2018年12月5日
*@see
*/
static void *FuncHandleEntryRtnVoidPtr(void * pArg)
{
	///@todo 这个函数内部的一个todolist
    void *pRst = NULL;
    Bind_t *pBind = (Bind_t *) pArg;
    if ((NULL != pBind->m_ClassCallBack.pClass)
            && (NULL != pBind->m_ClassCallBack.pCallback))
    {
        pRst = (pBind->m_ClassCallBack.pClass->*(pBind->m_ClassCallBack.pCallback))(pBind->pArg);

    }
    else if(NULL != pBind->m_pNormalHandle)
    {
        pRst = pBind->m_pNormalHandle(pBind->pArg);
    }
    return pRst;

}
/**
*@brief 无返回值的函数（普通函数与成员函数）入口点
*
* FuncHandleEntryRtnVoid我是一个详细的简介
*有换行
*@param void * pArg  
*@return static
*@author yccheng
*@todo 这个函数详述部分的一个todolist
*@see
*修改历史      :
  1.日    期   : 2018年12月5日
    作    者   : Litost_Cheng
    修改内容   : 新生成函数

*/
static void FuncHandleEntryRtnVoid(void * pArg)
{
    void *pRst = NULL;
    Bind_t *pBindNext = (Bind_t *) pArg;
    if ((NULL != pBindNext->m_ClassCallBack.pClass)
            && (NULL != pBindNext->m_ClassCallBack.pCallback))
    {
        pRst = (pBindNext->m_ClassCallBack.pClass->*(pBindNext->m_ClassCallBack.pCallback))(pBindNext->pArg);

    }
    else if(NULL != pBindNext->m_pNormalHandle)
    {
        pRst = pBindNext->m_pNormalHandle(pBindNext->pArg);
    }
    return ;

}

/**
*@brief 信号句柄
*@param int nSig  
*@return 
*@author yccheng
*修改历史      :
  1.日    期   : 2018年12月5日
    作    者   : Litost_Cheng
    修改内容   : 新生成函数

*/
void ThreadExitHandler(int nSig)
{
    DEBUG_TEST("this signal is [%d]\n", nSig);
    pthread_exit(0);
}



/**
*@brief 创建一个线程，并开始一个新的执行流
*@param const FuncHandle_t FuncHandle
*@param const void * pArg
*@return int
* 			0:启动线程成功
* 			-1:参数无效
* 			-2:之前的线程尚未退出
*@author yccheng
*修改历史      :
  1.日    期   : 2018年12月1日
    作    者   : Litost_Cheng
    修改内容   : 新生成函数

*/
int CThreadEvent::Start(void *pClass , CallBack FuncHandle, void * pArg)
{
    ///参数检测
    if (NULL == FuncHandle)
    {
        DEBUG_TEST("FuncHandle is NULL");
        return -1;
    }
    ///需要判断当前是否有线程正在运行int
    if (0 != m_Pid || !m_bThreadExit)
    {
        DEBUG_TEST("m_Pid is [%lu], m_bThreadExit is [%d] can't Create a new Thread!\n", (unsigned long)m_Pid, m_bThreadExit);
        return -2;
    }

    m_Bind.Reset();
    m_BindNext.Reset();

    m_BindNext.m_ClassCallBack.pClass = (null_class* )pClass;
    m_BindNext.m_ClassCallBack.pCallback = FuncHandle;
    m_BindNext.m_pNormalHandle = NULL;
    m_BindNext.pArg = pArg;

    m_Bind.m_ClassCallBack.pClass = (null_class* )this;
    m_Bind.m_ClassCallBack.pCallback = (CallBack)&CThreadEvent::_Start;
    m_Bind.m_pNormalHandle = NULL;
    m_Bind.pArg = (void *)&m_BindNext;

    ///明确设置线程的分离属性
    pthread_attr_t	 attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


    ///创建线程

    int nRst = pthread_create(&m_Pid, &attr, FuncHandleEntryRtnVoidPtr, &m_Bind);
    DEBUG_TEST("Create Thread Rst is [%d]\n", nRst);

    ///摧毁线程属性对象
    pthread_attr_destroy(&attr);
    return nRst;

}
/**
*@brief 创建一个线程，并开始一个新的执行流
*@param NormalFuncHandle FuncHandle  
*@param void *pArg                   
*@return int
* 			0:启动线程成功
* 			-1:参数无效
* 			-2:之前的线程尚未退出
*@author yccheng
*修改历史      :
  1.日    期   : 2018年12月5日
    作    者   : Litost_Cheng
    修改内容   : 新生成函数

*/
int CThreadEvent::Start(NormalFuncHandle FuncHandle, void *pArg)
{
    ///参数检测
    if (NULL == FuncHandle)
    {
        DEBUG_TEST("FuncHandle is NULL");
        return -1;
    }
    ///需要判断当前是否有线程正在运行int
    if (0 != m_Pid || !m_bThreadExit)
    {
        DEBUG_TEST("m_Pid is [%lu], m_bThreadExit is [%d] can't Create a new Thread!\n", (unsigned long)m_Pid, m_bThreadExit);
        return -2;
    }


    m_BindNext.m_ClassCallBack.pClass = NULL;
    m_BindNext.m_ClassCallBack.pCallback = NULL;
    m_BindNext.m_pNormalHandle = FuncHandle;
    m_BindNext.pArg = pArg;

    m_Bind.m_ClassCallBack.pClass = (null_class* )this;
    m_Bind.m_ClassCallBack.pCallback = (CallBack)&CThreadEvent::_Start;
    m_Bind.m_pNormalHandle = NULL;
    m_Bind.pArg = (void *)&m_BindNext;

    ///明确设置线程的分离属性
    pthread_attr_t	 attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


    ///创建线程

    int nRst = pthread_create(&m_Pid, &attr, FuncHandleEntryRtnVoidPtr, &m_Bind);
    DEBUG_TEST("Create Thread Rst is [%d]\n", nRst);
    ///摧毁线程属性对象
    pthread_attr_destroy(&attr);

    return nRst;
}

/**
*@brief 实际的线程执行流，由于在线程退出后，需要执行一些清除动作，因此需要对用户实际的控制流进行封
        装（用户的函数需要再次函数中调用），并设置必要的线程处理程序
*@param void * pArg
*@return void *
*@author yccheng
*修改历史      :
  1.日    期   : 2018年12月1日
    作    者   : Litost_Cheng
    修改内容   : 新生成函数

*/
void * CThreadEvent::_Start(void * pArg)
{

    ///判断参数
    if (NULL == pArg)
    {
        DEBUG_TEST("pArg is NULL, thread exit\n");
        pthread_exit((void *)-1);
    }

    ///打印线程ID，通过比较每次的ID是否一致，以判断资源是否已经释放
    DEBUG_TEST("Thread[%ld] Start\n", pthread_self());

#if USE_SIGNAL == 1
    ///设置信号回调句柄，用以取消线程
    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = ThreadExitHandler;
    sigaction(SIGUSR1, &actions, NULL);
#endif

    ///建立线程清理函数
    Bind_t Bind;
    Bind.m_ClassCallBack.pClass = (null_class* )this;
    Bind.m_ClassCallBack.pCallback = (CallBack)&CThreadEvent::PthreadCleanUp;
    Bind.m_pNormalHandle = NULL;
    Bind.pArg = NULL;
    pthread_cleanup_push(FuncHandleEntryRtnVoid, &Bind);


    ///线程正在运行，修改线程运行状态
    m_bThreadExit = false;
    ///DEBUG_TEST("Set m_bThreadExit = false\n");


	///XXX 由于安卓平台不支持pthread_cacel(),因此无法使用此种方式取消线程
#if USE_SIGNAL == 1
#else
        ///设置线程分离状态和分离类型
        int nRst = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (0 != nRst)
        {
            DEBUG_TEST("pthread_setcancelstate failed, Return[%d]\n", nRst);
            pthread_exit((void *)-2);
        }

        nRst = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
        if (0 != nRst)
        {
           DEBUG_TEST("pthread_setcanceltype failed, Return[%d]\n", nRst);
           pthread_exit((void *)-3);
        }
#endif




    void *pRst = NULL;
    Bind_t *pBind = (Bind_t *) pArg;
    if ((NULL != pBind->m_ClassCallBack.pClass)
            && (NULL != pBind->m_ClassCallBack.pCallback))
    {
        pRst = (pBind->m_ClassCallBack.pClass->*(pBind->m_ClassCallBack.pCallback))(pBind->pArg);

    }
    else if(NULL != pBind->m_pNormalHandle)
    {
        pRst = pBind->m_pNormalHandle(pBind->pArg);
    }
    else
    {
        DEBUG_TEST("Can't execute Func");
    }

    pthread_exit(pRst);

    ///调用线程清理函数
    pthread_cleanup_pop(1);

}



/**
*@brief 取消当前线程的执行
*@param    : none
*@return int
* 			0:取消线程成功
*			-1:取消线程失败
*@author yccheng
*修改历史      :
  1.日    期   : 2018年11月30日
    作    者   : Litost_Cheng
    修改内容   : 新生成函数

*/
int CThreadEvent::Cancel()
{
    ///需要在此处首先判断线程状态，判断当前线程是否存在
    ///由于在pthread_kill与其它线程再次创建该线程间可能存在一个时间窗口(经测试，线程ID存在复用情况)，因此可能需要我们对线程ID进行判断
    DEBUG_TEST("Cancel()\n");
    int nRst = -1;
    
    do
    {
    	if (m_bThreadExit)
    	{
    		nRst = 0;
    		DEBUG_TEST("m_bThreadExit is true !\n");
    		break;
    	}
        nRst = pthread_kill(m_Pid, 0);
        if (nRst != 0)
        {
            DEBUG_TEST("Thread [%ld] has exit!\n", m_Pid);
            nRst = 0;
            break;
        }
        ///尝试取消线程

		///XXX 由于安卓平台不支持pthread_cacel(),因此无法使用此种方式取消线程
#if USE_SIGNAL == 1
        nRst = pthread_kill(m_Pid, SIGUSR1);
        DEBUG_TEST("\n************\nSend Signal [SIGUSR1]!\n************\n");
#else
        nRst = pthread_cancel(m_Pid);
        DEBUG_TEST("\n************\nCall pthread_cancel()!\n************\n");

#endif
        

        if (nRst != 0)
        {
            DEBUG_TEST("Cancel Thread [%ld] failed,readson :[%d]\n", m_Pid, nRst);
            nRst = -1;
            break;
        }
        ///尝试取消线程
        nRst = pthread_cancel(m_Pid);
        if (nRst != 0)
        {
            DEBUG_TEST("Cancel Thread [%ld] failed,readson :[%d]\n", m_Pid, nRst);
            nRst = -1;
            break;
        }



        ///再次判断线程是否存在
        nRst = 0;
        while(0 == nRst && !m_bThreadExit)
        {
            nRst = pthread_kill(m_Pid, 0);
            usleep(500000);
            DEBUG_TEST("Try Get thread state again\n");
        }
        nRst = 0;


    }
    while(0);

    ///判断是否已成功取消线程
    if (0 == nRst  && m_bThreadExit)
    {
        DEBUG_TEST("Thread [%ld] exit!\n", m_Pid);
        m_Pid = 0;
        m_Bind.Reset();
        m_BindNext.Reset();
    }
    DEBUG_TEST("Cancel() Exit, nRst is [%d], m_Pid [%lu], m_bThreadExit is [%d]\n", nRst, m_Pid, m_bThreadExit);
    return nRst;
}


/**
*@brief 线程清理函数，处理线程退出后的善后工作
*@param void *
*@return void
*@author yccheng
*修改历史      :
  1.日    期   : 2018年12月1日
    作    者   : Litost_Cheng
    修改内容   : 新生成函数

*/
void * CThreadEvent::PthreadCleanUp(void *)
{

    DEBUG_TEST("PthreadCleanUp\n");
    ///Do Something


    ///设置线程退出标注，标识线程已执行完退出动作
    m_bThreadExit = true;
    m_Pid = 0;
    m_Bind.Reset();
    m_BindNext.Reset();    
    return NULL;
}

