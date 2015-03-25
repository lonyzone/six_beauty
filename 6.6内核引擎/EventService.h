//Download by http://www.NewXing.com
#ifndef EVENT_SERVICE_HEAD_FILE
#define EVENT_SERVICE_HEAD_FILE

#pragma once

#include "KernelEngine.h"

//////////////////////////////////////////////////////////////////////////

//�¼�������
class CEventService : public IEventService
{
	//��������
protected:
	HWND							m_hRichEdit;						//�ؼ����
	//CThreadLock						m_ThreadLock;						//�߳���
	CCriticalSection				m_CriticalSection;					//�߳�����
	bool							m_bEventService[EVENT_LEVEL_COUNT];	//��ʾ��־

	//״̬����
protected:
	int								m_nMaxLineCount;					//�������
	int								m_nReserveLineCount;				//��������

	//��������
public:
	//���캯��
	CEventService(void);
	//��������
	virtual ~CEventService(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() {delete this; }
	//�Ƿ���Ч
	//virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CEventService))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//���þ��
	virtual bool __cdecl SetRichEditHwnd(HWND hRichEdit);
	//���ü���
	virtual void __cdecl ConfigEventService(enTraceLevel TraceLevel, bool bShow);
	//�¼�֪ͨ
	virtual void __cdecl ShowEventNotify(LPCTSTR pszString, enTraceLevel TraceLevel);
};

//////////////////////////////////////////////////////////////////////////

//�¼������
class CEventTrace
{
	//��������
private:
	static IEventService					* m_pIEventService;			//�¼��ӿ�

	//��������
private:
	//���캯��
	CEventTrace() {}

	//���ܺ���
public:
	//���ýӿ�
	static bool SetEventService(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	static void * GetEventService(const IID & Guid, DWORD dwQueryVer);
	//���ü���
	static void ConfigEventService(enTraceLevel TraceLevel, bool bShow);
	//�¼�֪ͨ
	static void ShowEventNotify(LPCTSTR pszString, enTraceLevel TraceLevel);
};

//////////////////////////////////////////////////////////////////////////

#endif