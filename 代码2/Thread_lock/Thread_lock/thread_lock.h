//thread_lock.h
#ifndef THREAD_LOCK_HEAD_FILE
#define THREAD_LOCK_HEAD_FILE
#include<windows.h>
////////////////////////////////////////////////////////////////////////////

//�ٽ���ͬ����
class CThreadLock
{
	//��������
private:
	CRITICAL_SECTION				m_csLock;			//�ٽ�������

	//��������
public:
	//���캯��
	inline CThreadLock() {	::InitializeCriticalSection(&m_csLock);	};
	//��������
	inline ~CThreadLock() {	::DeleteCriticalSection(&m_csLock);	};

	//���ܺ���
public:
	//��������
	inline void Lock() {	::EnterCriticalSection(&m_csLock); };
	//��������
	inline void UnLock() {	::LeaveCriticalSection(&m_csLock); };
};

////////////////////////////////////////////////////////////////////////////

//��ȫͬ���������
class CThreadLockHandle
{
	//��������
private:
	int								m_nLockCount;		//��������
	CThreadLock						*m_pThreadLock;		//��������

	//��������
public:
	//���캯��
	CThreadLockHandle(CThreadLock *pThreadLock, bool bAutoLock=true);
	//��������
	virtual ~CThreadLockHandle();

	//���ܺ���
public:
	//��������
	void Lock();
	//��������
	void UnLock();
	//��ǹ��������
	int inline GetLockCount() {	return m_nLockCount; };
};

#endif