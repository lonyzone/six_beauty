#include "./HttpSessionMaster.h"

HttpSessionWorkBase::HttpSessionWorkBase(HANDLE hCompletionPort):m_hCompletionPort(hCompletionPort)
{
}

HttpSessionWorkBase::~HttpSessionWorkBase()
{
}

bool HttpSessionWorkBase::RepetionRun()
{
	//У�����
	if(m_hCompletionPort==NULL)
	{
		//��ɶ˿�δ��ʼ��
		return false;
	}
	//��������
	DWORD dwThancferred=0;
	OVERLAPPED *pOverLapped=NULL;
	HttpSessionMaster<HttpSessionWorkBase> *pQueueService=NULL;

	//�ȴ���ɶ˿�
	if(GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pQueueService,&pOverLapped,INFINITE))
	{
		//�ж��˳�
		if (pQueueService == NULL)	return false;

		//��ȡ����
		memset(m_cbBuffer,0,sizeof(m_cbBuffer));
		bool bGet = pQueueService->GetTask(m_cbBuffer,sizeof(m_cbBuffer));

		if(bGet==true)
		{
			//��������
			HandleTask(m_cbBuffer,dwThancferred);
		}

		return true;
	}
	return false;
}

void HttpSessionWorkBase::HandleTask(const void* pBuffer,UINT uSize)
{

}

UINT HttpSessionWorkBase::CurlWriteBuffer(char *buffer,UINT size,UINT nmemb,std::string* stream)
{
	UINT sizes = size*nmemb;
	if(stream == NULL) return 0;
	stream->append(buffer,sizes);
	return sizes;
}
