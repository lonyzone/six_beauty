#include<stdio.h>
#include<windows.h>
#include <process.h> 
#include"thread_lock.h"

const int aSize=10;
char szArr[aSize+1]={};

CThreadLock						threadLock;							//����CThreadLock���͵�ȫ�ֱ���

unsigned _stdcall threadFunc1(void*)
{
	CThreadLockHandle lockHandle(&threadLock);						//��Ҫʹ���ٽ����ǣ�����һ��CThreadLockHandle���͵ı��������������ڽ����Զ�����
	for(int s=0;s<aSize;s++)
	{
		szArr[s]='a';
		Sleep(1);
	}
	return 0;
}

unsigned _stdcall threadFunc2(void*)
{
	CThreadLockHandle lockHandle(&threadLock);						//��Ҫʹ���ٽ����ǣ�����һ��CThreadLockHandle���͵ı��������������ڽ����Զ�����
	for(int s=0;s<aSize;s++)
	{
		szArr[aSize-1-s]='b';
		Sleep(1);
	}
	return 0;
}

int main()
{
	memset(szArr,0,sizeof(szArr));
	HANDLE handle1=(HANDLE)_beginthreadex(NULL,0,threadFunc1,NULL,0,0);
	HANDLE handle2=(HANDLE)_beginthreadex(NULL,0,threadFunc2,NULL,0,0);

	WaitForSingleObject(handle1,INFINITE);
	WaitForSingleObject(handle2,INFINITE);

	printf("%s\n",szArr);
	CloseHandle(handle1);
	CloseHandle(handle2);

	return 0;
}


CThreadLock						m_ThreadLock=CThreadLock();
class threadtest
{
protected:
	static CThreadLock						m_ThreadLock;				//�߳���

public:
	static unsigned _stdcall threadFunction(void* pThreadData);
};
unsigned _stdcall threadtest::threadFunction(void* pThreadData)
{
	CThreadLockHandle lockHandle(&m_ThreadLock);				//�������ڽ����Զ�����
	//doSomething
	return 0;
}
