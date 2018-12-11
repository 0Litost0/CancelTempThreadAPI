#include <pthread.h>
#include <stdio.h>
#include <CThread.h>
#include <unistd.h>
void * NormalFunc_1(void *)
{
    int nCnt = 3;
    while(nCnt--)
    {
        printf("I am NormalFunc_1!\n");
        sleep(1);
    }
    return NULL;

}

void * NormalFunc_2(void *)
{
    int nCnt = 3;
    while(nCnt--)
    {
        printf("I am NormalFunc_2!\n");
        sleep(1);
    }
    return NULL;

}

class A
{
    public:
    void * MemberFunc_1(void *)
    {
        int nCnt = 3;
        while(nCnt--)
        {
            printf("I am A::MemberFunc_1!\n");
            sleep(2);
        }
        return NULL;

    }

    void * MemberFunc_2(void *)
    {
        int nCnt = 3;
        while(nCnt--)
        {
            printf("I am A::MemberFunc_2!\n");
            sleep(2);
        }
        return NULL;

    }
};

int main(void)
{
    A a;
    printf("main :sThread[%ld] Start\n", pthread_self());
    CThreadEvent TempThread;

    while ( 0 != TempThread.Start(NormalFunc_1, NULL));
    sleep(5);
    TempThread.Cancel();

    while ( 0 != TempThread.Start(&a, (CallBack)&A::MemberFunc_1, NULL));
    sleep(5);
    TempThread.Cancel();

    while ( 0 != TempThread.Start(NormalFunc_2, NULL));
    sleep(5);
    TempThread.Cancel();

    while ( 0 != TempThread.Start(&a, (CallBack)&A::MemberFunc_2, NULL));
    sleep(5);
    TempThread.Cancel();


    return 0;
}
