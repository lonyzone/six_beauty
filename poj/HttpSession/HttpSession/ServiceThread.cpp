#include <Process.h>
#include "ServiceThread.h"
//�ṹ����

//��������
struct tagThreadParameter
{
	bool			bSuccess;			//�Ƿ����
	HANDLE			hEventFinish;		//�¼����
	CServiceThread  *m_pServiceThread;  //�߳�ָ��
};

//////////////////////////////////////////////////////////////////////////

//************************************
// <p>Description: ���캯��</p>
//************************************
CThreadLockHandle::CThreadLockHandle(IThreadLock * pIThreadLock, bool bAutoLock)
{
	_ASSERT(pIThreadLock!=NULL);
	m_nLockCount=0;
	m_pIThreadLock=pIThreadLock;
	if (bAutoLock) Lock();
	return;
}

//************************************
// <p>Description: ��������</p>
//************************************
CThreadLockHandle::~CThreadLockHandle()
{
	while (m_nLockCount>0) UnLock();
}

//************************************
// <p>Description: ��������</p>
//
// <p>Returns:   void</p>
//************************************
void CThreadLockHandle::Lock()
{
	//Ч��״̬
	_ASSERT(m_nLockCount>=0);
	_ASSERT(m_pIThreadLock!=NULL);

	//��������
	m_nLockCount++;
	m_pIThreadLock->Lock();
}

//************************************
// <p>Description: ��������</p>
//
// <p>Returns:   void</p>
//************************************
void CThreadLockHandle::UnLock()
{
	//Ч��״̬
	_ASSERT(m_nLockCount>0);
	_ASSERT(m_pIThreadLock!=NULL);

	//�������
	m_nLockCount--;
	m_pIThreadLock->UnLock();
}

//////////////////////////////////////////////////////////////////////////

//************************************
// <p>Description: ���캯��</p>
//************************************
CServiceThread::CServiceThread(void)
{
	m_bRun=false;
	m_uThreadID=0;
	m_hThreadHandle=NULL;
}

//************************************
// <p>Description: ��������</p>
//************************************
CServiceThread::~CServiceThread(void)
{
	StopThread(INFINITE);
}

//************************************
// <p>Description: ״̬�ж�</p>
//
// <p>Returns:   bool</p>
//************************************
bool CServiceThread::IsRuning()
{
	if (m_hThreadHandle!=NULL)
	{
		DWORD dwRetCode=WaitForSingleObject(m_hThreadHandle,0);
		if (dwRetCode==WAIT_TIMEOUT) return true;
	}
	return false;
}

//************************************
// <p>Description: �����߳�</p>
//
// <p>Returns:   bool</p>
//************************************
bool CServiceThread::StartThead()
{
	//Ч��״̬
	if (IsRuning()) return false;

	//�������
	if (m_hThreadHandle!=NULL) 
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle=NULL;
		m_uThreadID=0;
	}

	//�������
	tagThreadParameter ThreadParameter;
	ThreadParameter.bSuccess=false;
	ThreadParameter.m_pServiceThread=this;
	ThreadParameter.hEventFinish=CreateEvent(NULL,FALSE,FALSE,NULL);
	if (ThreadParameter.hEventFinish==NULL) return false;

	//�����߳�
	m_bRun=true;
	m_hThreadHandle=(HANDLE)::_beginthreadex(NULL,0,ThreadFunction,&ThreadParameter,0,&m_uThreadID);

	//�ȴ��¼�
	WaitForSingleObject(ThreadParameter.hEventFinish,INFINITE);

	//�ر��¼�
	CloseHandle(ThreadParameter.hEventFinish);

	//�жϴ���
	if (ThreadParameter.bSuccess==false) 
	{
		StopThread();
		return false;
	}

	return true;
}

//************************************
// <p>Description: ֹͣ�߳�</p>
// <p>Parameters:  </p>
// <p>    DWORD dwWaitSeconds</p>
//
// <p>Returns:   bool</p>
//************************************
bool CServiceThread::StopThread(DWORD dwWaitSeconds)
{
	//ֹͣ�߳�
	if (IsRuning()==true)
	{
		m_bRun=false;
		DWORD dwRetCode=WaitForSingleObject(m_hThreadHandle,dwWaitSeconds);
		if (dwRetCode==WAIT_TIMEOUT) return false;
	}

	//���ñ���
	if (m_hThreadHandle!=NULL)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle=NULL;
		m_uThreadID=0;
	}

	return true;
}

//************************************
// <p>Description: ��ֹ�߳�</p>
// <p>Parameters:  </p>
// <p>    DWORD dwExitCode</p>
//
// <p>Returns:   bool</p>
//************************************
bool CServiceThread::TerminateThread(DWORD dwExitCode)
{
	//ֹͣ�߳�
	if (IsRuning()==true)
	{
		::TerminateThread(m_hThreadHandle,dwExitCode);
	}

	//���ñ���
	if (m_hThreadHandle!=NULL)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle=NULL;
		m_uThreadID=0;
	}

	return true;
}

//************************************
// <p>Description: Ͷ����Ϣ</p>
// <p>Parameters:  </p>
// <p>    UINT uMessage</p>
// <p>    WPARAM wParam</p>
// <p>    LPARAM lParam</p>
//
// <p>Returns:   bool</p>
//************************************
bool CServiceThread::PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//״̬Ч��
	if (m_uThreadID==0) return false;

	//Ͷ����Ϣ
	BOOL bSuccess=::PostThreadMessage(m_uThreadID,uMessage,wParam,lParam);
	return bSuccess?true:false;
}

//************************************
// <p>Description: �̺߳���</p>
// <p>Parameters:  </p>
// <p>    LPVOID pThreadData</p>
//
// <p>Returns:   unsigned __stdcall</p>
//************************************
unsigned __stdcall CServiceThread::ThreadFunction(LPVOID pThreadData)
{
	//��ȡ����
	_ASSERT(pThreadData!=NULL);
	tagThreadParameter * pThreadParameter=(tagThreadParameter *)pThreadData;
	CServiceThread * pThread=pThreadParameter->m_pServiceThread;

	//�����¼�
	try 
	{
		pThreadParameter->bSuccess=pThread->OnThreadStratEvent(); 
	} 
	catch (...) 
	{ 
		pThreadParameter->bSuccess=false; 
	}

	//�����¼�
	bool bSuccess=pThreadParameter->bSuccess;
	_ASSERT(pThreadParameter->hEventFinish!=NULL);
	SetEvent(pThreadParameter->hEventFinish);

	//�����߳�
	if (bSuccess==true)
	{
		while (pThread->m_bRun)
		{
			try
			{
				if (pThread->RepetitionRun()==false) break;
			}
			catch (...) {}
		}
	}

	//ֹͣ�¼�
	try 
	{ 
		pThread->OnThreadStopEvent(); 
	} 
	catch (...) 
	{
	}

	//��ֹ�߳�
	_endthreadex(0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
