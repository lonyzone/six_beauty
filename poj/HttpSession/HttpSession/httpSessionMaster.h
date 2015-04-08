#pragma once

#include "ServiceThread.h"
#include <string>
#include <vector>
#include <list>

const unsigned int MAX_DATA_PACKET = 10240;

//����http�Ķ����߳���
class HttpSessionWorkBase:public CServiceThread
{
protected:
	HANDLE			m_hCompletionPort;					//��ɶ˿�
	//��������
private:
	BYTE			m_cbBuffer[MAX_DATA_PACKET];		//���ջ���

public:
	HttpSessionWorkBase(HANDLE hCompletionPort);
	virtual ~HttpSessionWorkBase();
private:
	//���к���
	virtual bool RepetionRun();
	//��������,��Ҫ�ɼ̳�HttpSessionWorkBase����ʵ��
	virtual void HandleTask(const void* pBuffer,unsigned int uSize);

	static UINT CurlWriteBuffer(char *buffer,UINT size,UINT nmemb,std::string* stream);
};

 
template<class T>
class HttpSessionMaster
{
	typedef std::list<char*>	TaskQueue;
public:
	bool StartService();
	void StopService();
	bool SetTaskDataSize(UINT uSize);
	void PostTask(void* pBuffer,unsigned int uSize);
public:
	HttpSessionMaster();
	~HttpSessionMaster();
	bool GetTask(void *pBuffer,unsigned int &uSize);
private:
	HANDLE				m_hCompletionPort;		
	CThreadLock			m_ThreadLock;
	TaskQueue			m_TaskQueue;		//�������
	TaskQueue			m_FreeTaskQueue;	//�����ڴ�أ���ʡnew�Ŀ���
	
	std::vector<HttpSessionWorkBase*>		m_WorkerThread;		//�����߳�
	UINT								m_uPerTaskDataSize;	//�����������ݵĴ�С
};

template<class T>
HttpSessionMaster<T>::HttpSessionMaster():m_hCompletionPort(NULL)
{
}

template<class T>
HttpSessionMaster<T>::~HttpSessionMaster()
{
	StopService();
}

template<class T>
bool HttpSessionMaster<T>::StartService()
{
	ASSERT(m_hCompletionPort == NULL);

	try
	{
		//��ȡϵͳcpu����
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		UINT uThreadCount = SystemInfo.dwNumberOfProcessors * 2;

		//������ɶ˿�
		m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,SystemInfo.dwNumberOfProcessors);

		for(int i=0;i<uThreadCount;++i)
		{
			HttpSessionWorkBase *pWork = new T(m_hCompletionPort);
			if (pWork == NULL)
				throw ("�����̴߳���ʧ��!");
			m_WorkerThread.push_back(pWork);
		}
		for(int i=0;i<)
	}
	catch (...)
	{
		//������Exception
		ASSERT(false);
		return false;
	}
	return true;
}

template<class T>
void HttpSessionMaster<T>::StopService()
{
	if(m_hCompletionPort != NULL)
	{
		for(int i=0;i<(int)m_WorkerThread.size();++i)
			PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
	}

	for(int i=0;i<(int)m_WorkerThread.size();++i)
	{
		HttpSessionWorkBase *pWork = m_WorkerThread[i];
		pWork->StopThread();
		SafeDelete(pWork);
	}

	//�ر���ɶ˿�handle
	if(m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}
}

