// IOCP������noclass.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<vector>
#include<cstdio>
#include <winsock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <mswsock.h>    //΢����չ�����

#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define SOCK_BUF_SIZE   8192
#define SERVERPORT	 12345

//����IO��������
typedef enum _operation_type
{
	ACCEPT_TYPE,
	SEND_TYPE,
	RECV_TYPE,
	NULL_TYPE
}OPERATION_TYPE;

//������ɶ˿��еĵ�I/O���ݽṹ�壬ÿһ��I/O������Ӧһ��_per_io_data
typedef struct _per_io_data
{	
	OVERLAPPED	m_overlapped;		//ϵͳ���첽IO��Ϣ�ṹ��
	SOCKET	m_socket;				//����I/O������ʹ�õ�socket
	WSABUF	m_databuf;				//�洢���ݻ��������������ص��������ݲ���
	char	m_buffer[SOCK_BUF_SIZE];//��ӦWSABUF��Ļ�����
	OPERATION_TYPE	m_optype;				//�����������
	//��ʼ��
	_per_io_data()
	{
		memset(&m_overlapped,0,sizeof(OVERLAPPED));
		memset(m_buffer,0,SOCK_BUF_SIZE);
		m_socket=INVALID_SOCKET;
		m_databuf.len=SOCK_BUF_SIZE;;
		m_databuf.buf=m_buffer;
		m_optype=NULL_TYPE;
	}
	~_per_io_data()
	{
		if(m_socket!=INVALID_SOCKET)
		{
			closesocket(m_socket);
			m_socket=INVALID_SOCKET;
		}
	}
	void ResetBuffer()
	{
		ZeroMemory(m_buffer,SOCK_BUF_SIZE);
	}
}per_io_data,*p_per_io_data;

//���嵥������ݽṹ�壬ÿ�����������Ӧһ��socket��ÿ��socket���Զ�Ӧ���I/O����
typedef struct _per_socket_data
{
	SOCKET	m_socket;		//��Ӧ�ͻ��˵��׽���
	SOCKADDR_IN	m_clientaddr;	//��Ӧ�ͻ��˵ĵ�ַ
	vector<p_per_io_data>	m_vtiodata;	//socket��I/O���������ݽṹ��
	//����listensocket������socket,��ŵ���acceptIO����
	//����clientsocket���ͻ��ˣ�,���recv��send��IO����
	_per_socket_data()
	{
		m_socket=INVALID_SOCKET;
		memset(&m_clientaddr,0,sizeof(m_clientaddr));
	}
	~_per_socket_data()
	{
		if(m_socket!=INVALID_SOCKET)
		{
			//ѭ��ɾ���ͻ���
			for(vector<p_per_io_data>::iterator iter=m_vtiodata.begin();iter!=m_vtiodata.end();iter++)
			{
				delete *iter;
				iter=m_vtiodata.erase(iter);
			}
			//�ر�socket
			closesocket(m_socket);
		}
		for(vector<p_per_io_data>::iterator iter=m_vtiodata.begin();iter!=m_vtiodata.end();)
		{
			delete *iter;				//��һ�ν�Ӧ��ȡ�õ�I/O�ṹ���ַ���ڶ���ȡ�ýṹ��
			iter=m_vtiodata.erase(iter);
		}
	}//~_per_socket_data;
	p_per_io_data add_new_iodata()
	{
		p_per_io_data newiodata=new per_io_data;
		m_vtiodata.push_back(newiodata);
		return newiodata;

	}
	void dele_iodata(p_per_io_data deledata)
	{
		for(vector<p_per_io_data>::iterator iter=m_vtiodata.begin();iter!=m_vtiodata.end();)
		{
			if(*iter==deledata)
			{
				delete deledata;
				iter=m_vtiodata.erase(iter);
			}
			else
				iter++;
		}
	}
}per_socket_data,*p_per_socket_data;	
//������ɶ˿��еĵ�I/O���ݽṹ�壬ÿһ��I/O������Ӧһ��_per_io_data
typedef struct work_thread_struct
{
	int threadid;				//�߳�id
	HANDLE m_completionport;	//��ɶ˿ھ��
}work_struct;
DWORD WINAPI WorkThread(LPVOID lParam);		//�������߳�

LPFN_ACCEPTEX	m_lpAcceptEx=NULL;		//acceptex������ָ��
LPFN_GETACCEPTEXSOCKADDRS m_lpGetAcceptExSockAddrs=NULL;		//GetAcceptSockAddrs������ָ��
CRITICAL_SECTION m_cslock;				//�ٽ���
HANDLE m_EndEvent;						//�����¼�
vector<per_socket_data*> lstclntdata;	//�ͻ��˵�socket���������ͷ��ڴ�
SOCKET m_ListenSocket;					//�����׽���


//Ͷ��recv����
bool postrecv(per_io_data* pIodata)
{
	//WSARecv�Ĳ���׼��
	DWORD dwRecvBytes=0;
	DWORD dwInfo=0;
	WSABUF *databuf=&pIodata->m_databuf;
	pIodata->ResetBuffer();
	pIodata->m_optype=RECV_TYPE;
	OVERLAPPED *p_ol=&pIodata->m_overlapped;
	//Ͷ��WSARecv����
	if(WSARecv(pIodata->m_socket,databuf,1,&dwRecvBytes,&dwInfo,p_ol,NULL)==SOCKET_ERROR)
	{
		printf("Ͷ��recv����ʧ��...\n");
		return false;
	}
	return true;
}
//Ͷ��accept����
bool postaccept(per_socket_data* m_socketdata,per_io_data* pIodata)
{
	if(m_socketdata->m_socket==INVALID_SOCKET)
	{
		printf("�����׽����д���...\n");
		return false;
	}
	
	//ΪacceptEx׼������
	DWORD bytereceived=0;
	pIodata->m_optype = ACCEPT_TYPE; 
	WSABUF* databuf=&pIodata->m_databuf;
	OVERLAPPED *p_ol=&pIodata->m_overlapped;
	//	pIodata->m_databuf.len=SOCK_BUF_SIZE;			//֮ǰ�ռ�Ϊ0����Ϊ��ʼ��ʱ��ΪSOCK_BUF_SIZE�ͺ�
	//Ͷ��accpetEx����,m_lpAcceptEx��acceptExͨ��WSAIoctl��õĺ���ָ��
	
	//ΪacceptEx׼��socket��acceptEx��accept��Ч���ص㣩
	pIodata->m_socket=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if((pIodata->m_socket)==INVALID_SOCKET)
	{
		printf("ΪacceptEx�����׽���ʧ��...\n");
		return false;
	}
	if(FALSE==m_lpAcceptEx(m_socketdata->m_socket,pIodata->m_socket,databuf->buf, databuf->len - ((sizeof(SOCKADDR_IN)+16)*2),   
		sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &bytereceived,p_ol))
	{
		if(WSA_IO_PENDING != WSAGetLastError())  
		{  
			printf("Ͷ��acceptExʧ�ܣ��������:%d\n",WSAGetLastError()); 
			return false;  
		}  
	}
	//�ȴ��߳̽���
	return true;
}


int main()
{
	int m_nThreads;							//�����̵߳ĸ���
	HANDLE *m_workthhdle=NULL;				//�����߳̾��
	per_socket_data* m_socketdata=NULL;		//����socketdata
	struct sockaddr_in	m_localsockaddr;	//���ص�ַ
	HANDLE m_completionport=INVALID_HANDLE_VALUE;				//��ɶ˿ھ��
	//����socket��
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=NO_ERROR)
	{
		printf("��socket��̬���ӿ����.\n");
		return false;  
	}else
		printf("��socket��̬���ӿ����.\n");

	/////////////////////////////////////////////
	//��ʼ�����������˳��¼�
	InitializeCriticalSection(&m_cslock);
	m_EndEvent=CreateEvent(NULL,true,false,NULL);

	/////////////////////////////////////////////
	//������ɶ˿�
	if((m_completionport=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0))==NULL)
	{
		printf("�����˿�ʧ��.\n");
		return false;
	}else
		printf("������ɶ˿ڳɹ�.\n");



	/////////////////////////////////////////////
	//����work�̳߳�
	SYSTEM_INFO si;
	GetSystemInfo(&si);	//��ȡϵͳ��Ϣ
	//����ϵͳ�������������������߳�
	m_nThreads=si.dwNumberOfProcessors*2+2;			//��Ҫ�����Ľ�������ɾ��ʱ��Ҫ��
	//��ʼ���������߳�
	m_workthhdle=new HANDLE[m_nThreads];
	//�����������߳�
	for(int i=0;i<m_nThreads;i++)
	{
		work_struct *workstruct=new work_struct;
		workstruct->threadid=i+1;
		workstruct->m_completionport=m_completionport;
		if((m_workthhdle[i]=CreateThread(NULL,0,WorkThread,(void*)workstruct,0,NULL))==NULL)
		{
			printf("�̳߳ش���ʧ��...\n");
			return false;  
		}
	}

	/////////////////////////////////////////////
	// �������ڼ�����Socket����Ϣ
	m_socketdata=new per_socket_data;
	//��������socket
	m_ListenSocket=m_socketdata->m_socket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if((m_socketdata->m_socket)==INVALID_SOCKET)
	{
		printf("����socket��ʼ��ʧ��...\n");
		return false;
	}else
		printf("����socket�����ɹ�.\n");
	//��listensocket�󶨵���ɶ˿���
	if(NULL==CreateIoCompletionPort((HANDLE)m_socketdata->m_socket,m_completionport,(DWORD)m_socketdata,0))
	{
		printf("��listensocket����ɶ˿�ʧ��....\n");
		if(m_socketdata->m_socket!=INVALID_SOCKET)
			closesocket(m_socketdata->m_socket);
		return false;
	}else
		printf("��listensocket����ɶ˿����.\n");
	//���ñ���addr
	memset(&m_localsockaddr,0,sizeof(m_localsockaddr));
	m_localsockaddr.sin_family=AF_INET;
	m_localsockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	m_localsockaddr.sin_port=htons(SERVERPORT);
	//�󶨼���socket�Ͷ˿�
	if(bind(m_socketdata->m_socket,(SOCKADDR*)&m_localsockaddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR)
	{
		printf("����socket�Ͷ˿ڰ�ʧ��...\n");
		return false;
	}
	//������ʼ
	if(listen(m_socketdata->m_socket,SOMAXCONN)!=0)
	{
		printf("��������ʧ��...\n");
		return false;
	}

	/////////////////////////////////////////////
	//��δ��ȡaccpetex�����͵��������Ľϴ�����WSAIoctl��ȡ����ָ��
	// AcceptEx �� GetAcceptExSockaddrs ��GUID�����ڵ�������ָ��
	GUID GuidAcceptEx = WSAID_ACCEPTEX;  
	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS; 
	DWORD acByte=0;
	//�����ȡGetAcceptExSockAddrs�ĵ�ַ
	DWORD dwBytes = 0;  
	if(SOCKET_ERROR == WSAIoctl(
		m_socketdata->m_socket, 
		SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidAcceptEx, 
		sizeof(GuidAcceptEx), 
		&m_lpAcceptEx, 
		sizeof(m_lpAcceptEx), 
		&dwBytes, 
		NULL, 
		NULL))  
	{
		printf("AcceptEx������ȡʧ��: %d\n", WSAGetLastError());  
		return false;
	}
	if(WSAIoctl(m_socketdata->m_socket,SIO_GET_EXTENSION_FUNCTION_POINTER,&GuidGetAcceptExSockAddrs,sizeof(GuidGetAcceptExSockAddrs),
		&m_lpGetAcceptExSockAddrs,sizeof(m_lpGetAcceptExSockAddrs),&dwBytes,0,0)==SOCKET_ERROR)
	{
		printf("GetAcceptExSockAddrs������ȡʧ��: %d\n", WSAGetLastError());  ;
		return false;
	}
	/////////////////////////////////////////////
	//acceptEx����ɶ˿���Ͷ�ݶ������	
	for(int i=0;i<14;i++)
	{
		per_io_data *periodata=m_socketdata->add_new_iodata();
		if(postaccept(m_socketdata,periodata)==false)
		{
			m_socketdata->dele_iodata(periodata);
			return false;
		}
	}
	printf("14��accept����Ͷ�����...\n");
	printf("��ʼ����ģ��:\n");

	WaitForMultipleObjects(m_nThreads,m_workthhdle,TRUE,INFINITE);
	closesocket(m_ListenSocket);
	WSACleanup();
	return 0;
}

//�������߳�workthread��ʵ��
DWORD WINAPI WorkThread(LPVOID lpParam)
{
	work_struct* workstruct=(work_struct*)lpParam;
	int threadid=(int)workstruct->threadid;
	HANDLE m_completionport=(HANDLE)workstruct->m_completionport;
	//	HANDLE m_completionport=(HANDLE)lpParam;
	LPOVERLAPPED	pOverapped=NULL;
	per_socket_data	*pSocketdata=NULL;
	per_io_data	*pIodata=NULL;
	DWORD	dwBytesTransfered=0;

	printf("�����߳�:%d	����...\n",threadid);
	//ѭ����������
	while(WAIT_OBJECT_0!=WaitForSingleObject(&m_EndEvent,0))
	{
		BOOL bRet=GetQueuedCompletionStatus(
			m_completionport,
			&dwBytesTransfered,
			(PULONG_PTR)&pSocketdata,
			(LPOVERLAPPED*)&pOverapped,
			INFINITE);
		if(bRet==0)
		{		
			printf("�����߳�GetQueuedCompletionStatus����,������:%d\n",WSAGetLastError());			
			return false;
		}//if
		pIodata=(per_io_data*)CONTAINING_RECORD(pOverapped,_per_io_data,m_overlapped);
		if(dwBytesTransfered==0 && ( pIodata->m_optype==RECV_TYPE || pIodata->m_optype==SEND_TYPE))
		{
			//�ͷŵ��߿ͻ��˵���Դ
			closesocket(pSocketdata->m_socket);
			delete pSocketdata;
			delete pIodata;
			continue;
		}//if
		switch(pIodata->m_optype)
		{
			//�����߳�workthread�ж�accpet_type�Ĳ���
		case ACCEPT_TYPE:
			{
				printf("ACCEPT_TYPE��IO������ʼ\n");
				//ͨ��GetAcceptExSockAddrs()��õ�ַ��Ϣ
				SOCKADDR_IN *LocalAddr=NULL;
				SOCKADDR_IN *ClientAddr=NULL;
				int localLen=sizeof(SOCKADDR_IN),clientLen=sizeof(SOCKADDR_IN);
				m_lpGetAcceptExSockAddrs(pIodata->m_databuf.buf,pIodata->m_databuf.len-((sizeof(SOCKADDR_IN)+16)*2),
					sizeof(SOCKADDR_IN)+16,sizeof(SOCKADDR_IN)+16,(LPSOCKADDR*)LocalAddr,&localLen,(LPSOCKADDR*)ClientAddr,&clientLen);

				printf("�û�%s��½������...\n",inet_ntoa(ClientAddr->sin_addr));
				printf("�û�%s\t:%s.\n",inet_ntoa(ClientAddr->sin_addr),pIodata->m_databuf.buf);
				//����client��socketdata��Ϣ
				per_socket_data* pnew_sockdata=new per_socket_data;
				pnew_sockdata->m_socket=pIodata->m_socket;
				memcpy(&pnew_sockdata->m_clientaddr,ClientAddr,sizeof(SOCKADDR_IN));			//����Ҫע�ⲻ��ֱ�Ӹ�ֵ��ָ�븳ֵ������////////////////////
				//��clientsocket����ɶ˿���
				if(NULL==CreateIoCompletionPort((HANDLE)pnew_sockdata->m_socket,m_completionport,(DWORD)pnew_sockdata,0))
				{
					printf("��clientsocket(%d)����ɶ˿�ʧ��....\n",inet_ntoa(ClientAddr->sin_addr));
					if(pnew_sockdata->m_socket!=INVALID_SOCKET)
						closesocket(pnew_sockdata->m_socket);
					return false;
				}
				//Ϊ���clientͶ�ݵ�һ��recv����
				per_io_data* pnew_iodata=pnew_sockdata->add_new_iodata();
				pnew_iodata->m_optype=RECV_TYPE;
				pnew_iodata->m_socket=pnew_sockdata->m_socket;
				if(postrecv(pnew_iodata)==false)
				{
					printf("Ϊclientsocket(%d)Ͷ�ݵ�һ��recv����ʧ��...\n",inet_ntoa(ClientAddr->sin_addr));
					pnew_sockdata->dele_iodata(pnew_iodata);
					return false;
				}
				//���½���clientdata����lstclntdata��
				EnterCriticalSection(&m_cslock);
				lstclntdata.push_back(pnew_sockdata);
				LeaveCriticalSection(&m_cslock);
				//�������ݻ�����������Ͷ��accpetEx����
				pIodata->ResetBuffer();
				postaccept(pSocketdata,pIodata);
				printf("ACCEPT_TYPE��IO��������\n");
				break;
			}
		case RECV_TYPE:
			//�����߳�workthread�ж�recv�Ĳ���
			printf("�ͻ�(%d):\n\t%s\n",inet_ntoa(pSocketdata->m_clientaddr.sin_addr),pIodata->m_databuf.buf);
			postrecv(pIodata);
			break;
		case SEND_TYPE:
			//		mythis->_doSend(pSocketdata,pIodata);
			break;
		default:
			printf("workthread�����쳣...\n");
			break;			
		}//switch						
	}//while
	printf("�����߳�:%d	�˳�...\n",threadid);
	delete workstruct;
	return 0;
}
