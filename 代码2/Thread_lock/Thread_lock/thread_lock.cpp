//thread_lock.cpp   programed by sany
//2014.9.2
//callme:sanyue9394@163.com


#include "thread_lock.h"
#include<assert.h>
////////////////////////////////////////////////////////////////////////////

//��ȫͬ���������
//���캯��
CThreadLockHandle::CThreadLockHandle(CThreadLock *pThreadLock, bool bAutoLock)
{
	assert(pThreadLock!=NULL);
	m_nLockCount=0;
	m_pThreadLock=pThreadLock;
	if(bAutoLock)	Lock();
	return;
}
//��������
CThreadLockHandle::~CThreadLockHandle()
{
	while(m_nLockCount>0) UnLock();								//�������ڽ����Զ�����
}

//��������
void CThreadLockHandle::Lock()
{
	//У��״̬
	assert(m_nLockCount>=0);
	assert(m_pThreadLock!=NULL);

	//��������
	m_nLockCount++;
	m_pThreadLock->Lock();
}

//��������
void CThreadLockHandle::UnLock()
{
	//У��״̬
	assert(m_nLockCount>0);
	assert(m_pThreadLock!=NULL);

	//���״̬
	m_nLockCount--;
	m_pThreadLock->UnLock();
}

