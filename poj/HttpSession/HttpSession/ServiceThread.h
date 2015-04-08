#ifndef _SERVICE_THREAD_H
#define _SERVICE_THREAD_H

#include <atlcore.h>


//ͬ������ӿ�
struct IThreadLock
{
	//��������
	virtual void Lock()=0;
	//�������� 
	virtual void UnLock()=0;
};

//�߳̽ӿ�
struct IServiceThread
{
	//״̬�ж�
	virtual bool IsRuning()=0;
	//�����߳�
	virtual bool StartThead()=0;
	//ֹͣ�߳�
	virtual bool StopThread(unsigned long dwWaitSeconds)=0;
	//��ֹ�߳�
	virtual bool TerminateThread(unsigned long dwExitCode)=0;
};

//////////////////////////////////////////////////////////////////////////

//�ٽ���ͬ����
class CThreadLock : public IThreadLock
{
	//��������
	private:
		CRITICAL_SECTION     m_csLock;		//�ٽ����

		//��������
	public:
		//���캯��
		inline CThreadLock() { ::InitializeCriticalSection(&m_csLock); }
		//��������
		inline ~CThreadLock() { ::DeleteCriticalSection(&m_csLock); }

		//���ܺ���
	public:
		//��������
		virtual inline void Lock() { ::EnterCriticalSection(&m_csLock); }
		//�������� 
		virtual inline void UnLock() { ::LeaveCriticalSection(&m_csLock); }
};

//////////////////////////////////////////////////////////////////////////

//��ȫͬ���������
class CThreadLockHandle
{
	//��������
	private:
		int				m_nLockCount;		//��������
		IThreadLock     *m_pIThreadLock;	//��������

		//��������
	public:
		//���캯��
		CThreadLockHandle(IThreadLock * pIThreadLock, bool bAutoLock=true);
		//��������
		virtual ~CThreadLockHandle();

		//���ܺ���
	public:
		//��������
		void Lock();
		//�������� 
		void UnLock();
		//��ȡ��������
		int inline GetLockCount() { return m_nLockCount; }
};

//////////////////////////////////////////////////////////////////////////

//�̶߳�����
class CServiceThread : public IServiceThread
{
	//��������
	private:
		volatile bool		m_bRun;				//���б�־
		UINT				m_uThreadID;		//�̱߳�ʶ
		HANDLE				m_hThreadHandle;    //�߳̾��

		//��������
	protected:
		//���캯��
		CServiceThread(void);
		//��������
		virtual ~CServiceThread(void);

		//�ӿں���
	public:
		//��ȡ״̬
		virtual bool IsRuning();
		//�����߳�
		virtual bool StartThead();
		//ֹͣ�߳�
		virtual bool StopThread(unsigned long dwWaitSeconds=INFINITE);
		//��ֹ�߳�
		virtual bool TerminateThread(unsigned long dwExitCode);

		//���ܺ���
	public:
		//��ȡ��ʶ
		UINT GetThreadID() { return m_uThreadID; }
		//��ȡ���
		HANDLE GetThreadHandle() { return m_hThreadHandle; }
		//Ͷ����Ϣ
		bool PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);

		//�¼�����
	private:
		//��ʼ�¼�
		virtual bool OnThreadStratEvent() { return true; }
		//ֹͣ�¼�
		virtual bool OnThreadStopEvent() { return true; }

		//�ڲ�����
	private:
		//���к���
		virtual bool RepetitionRun()=0;
		//�̺߳���
		static unsigned __stdcall ThreadFunction(LPVOID pThreadData);
};

//////////////////////////////////////////////////////////////////////////

#endif

