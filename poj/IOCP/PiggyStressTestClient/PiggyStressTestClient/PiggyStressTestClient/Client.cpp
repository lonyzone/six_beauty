#include "StdAfx.h"
#include "Client.h"
#include "MainDlg.h"

#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}

CClient::CClient(void):
			m_strServerIP(DEFAULT_IP),
			m_strLocalIP(DEFAULT_IP),
			m_nThreads(DEFAULT_THREADS),
			m_pMain(NULL),
			m_nPort(DEFAULT_PORT),
			m_strMessage(DEFAULT_MESSAGE),
			m_phWorkerThreads(NULL),
			m_hConnectionThread(NULL),
			m_hShutdownEvent(NULL),
			m_hRecvThread(NULL)

{
	SetThreads(1);
}

CClient::~CClient(void)
{
	this->Stop();
}

//////////////////////////////////////////////////////////////////////////////////
//	�������ӵ��߳�
DWORD WINAPI CClient::_ConnectionThread(LPVOID lpParam)
{
	THREADPARAMS_Client* pParams = (THREADPARAMS_Client*) lpParam;
	CClient* pClient = (CClient*)pParams->pClient;

	TRACE("_AccpetThread������ϵͳ������...\n");

	pClient->EstablishConnections();

	TRACE(_T("_ConnectionThread�߳̽���.\n"));

	RELEASE(pParams);	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ���ڽ�����Ϣ���߳�
DWORD WINAPI CClient::_RecvThread(LPVOID lpParam)
{
	THREADPARAMS_Client *pParams = (THREADPARAMS_Client *)lpParam;
	CClient* pClient = (CClient*) pParams->pClient;

	pClient->RecvConnect();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ���ڷ�����Ϣ���߳�
DWORD WINAPI CClient::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER *pParams = (THREADPARAMS_WORKER *)lpParam;
	CClient* pClient = (CClient*) pParams->pClient;

	char szTemp[MAX_BUFFER_LEN];
	memset( szTemp,0,sizeof(szTemp) );
	char szRecv[MAX_BUFFER_LEN];
	memset(szRecv,0,MAX_BUFFER_LEN);

	int nBytesSent = 0;
	int nBytesRecv = 0;

	//CopyMemory(szTemp,pParams->szBuffer,sizeof(pParams->szBuffer));

	// �������������Ϣ
	sprintf( szTemp,("��1����Ϣ��%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("���󣺷���1����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	
	TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	pClient->ShowMessage("�������������Ϣ�ɹ�: %s", szTemp);

	Sleep( 3000 );

	// �ٷ���һ����Ϣ
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("��2����Ϣ��%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("���󣺷��͵�2����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	
	
	TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	pClient->ShowMessage("�������������Ϣ�ɹ�: %s", szTemp);


	Sleep( 3000 );
	
	// ����3����Ϣ
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("��3����Ϣ��%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("���󣺷��͵�3����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	

	TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	pClient->ShowMessage("�������������Ϣ�ɹ�: %s", szTemp);


	if( pParams->nThreadNo==pClient->m_nThreads )
	{
		pClient->ShowMessage(_T("���Բ��� %d ���߳����."),pClient->m_nThreads);
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
// ��������
bool  CClient::EstablishConnections()
{
	DWORD nThreadID;

	m_phWorkerThreads = new HANDLE[m_nThreads];
	m_pParamsWorker = new THREADPARAMS_WORKER[m_nThreads];

	// �����û����õ��߳�����������ÿһ���߳����������������������̷߳�������
	for (int i=0; i<m_nThreads; i++)
	{
		// �����û���ֹͣ�¼�
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hShutdownEvent, 0))
		{
			TRACE(_T("���յ��û�ֹͣ����.\n"));
			return true;
		}
		
		// ���������������
		if( !this->ConnetToServer(&m_pParamsWorker[i].sock,m_strServerIP,m_nPort) )
		{
			ShowMessage(_T("���ӷ�����ʧ�ܣ�"));
			CleanUp();
			return false;
		}

		m_pParamsWorker[i].nThreadNo = i+1;
		sprintf(m_pParamsWorker[i].szBuffer, "%d���߳� �������� %s", i+1, m_strMessage.GetString() );

		Sleep(10);

		// ������ӷ������ɹ����Ϳ�ʼ�����������̣߳������������ָ������
		m_pParamsWorker[i].pClient = this;
		m_phWorkerThreads[i] = CreateThread(0, 0, _WorkerThread, (void *)(&m_pParamsWorker[i]), 0, &nThreadID);
	}

	return true;
}

bool CClient::RecvConnect()
{
	char szRecv[MAX_BUFFER_LEN]={0};
	memset(szRecv,0,MAX_BUFFER_LEN);

	SOCKET   RecvSock;  

	// ���������������
	if( !this->ConnetToServer(&RecvSock,m_strServerIP,m_nPort) )
	{
		ShowMessage(_T("���ӷ�����ʧ�ܣ�"));
		CleanUp();
		return false;
	}

	// ����Recvע����Ϣ
	char szSend[MAX_BUFFER_LEN]={0};
	memset(szSend,0,MAX_BUFFER_LEN);
	lstrcpyn(szSend,"RecvThread",sizeof(szSend));
	int nBytesSent = send(RecvSock, szSend, strlen(szSend), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("���󣺷���Recv�߳�ע����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}

	SOCKADDR_IN    addrSender;
	int addrSize = sizeof(SOCKADDR_IN);

	struct fd_set fds; 
	struct timeval timeout={3,0}; //select�ȴ�3�룬3����ѯ��Ҫ����������0 
	while(true)
	{ 
		FD_ZERO(&fds); //ÿ��ѭ����Ҫ��ռ��ϣ������ܼ���������仯 
		FD_SET(RecvSock,&fds); //��������� 
		switch(select(0,&fds,NULL,NULL,&timeout)) //selectʹ�� 
		{ 
		case -1: 
			{
				//select����
				return false;
			}
		case 0: 
			{
				// �����û���ֹͣ�¼�
				if(WAIT_OBJECT_0 == WaitForSingleObject(m_hShutdownEvent, 0))
				{
					TRACE(_T("���յ��û�ֹͣ����.\n"));
					return 0;
				}	
				break;
			}
		default: 
			if(FD_ISSET(RecvSock,&fds)) //����sock�Ƿ�ɶ������Ƿ������������� 
			{ 
				ZeroMemory(szRecv,sizeof(szRecv)/sizeof(szRecv[0]));
				int nBytesRecv = recvfrom(RecvSock,szRecv,sizeof(szRecv),0,(sockaddr*)&addrSender,&addrSize);
				if(nBytesRecv > 0)
				{
					ShowMessage(_T("��Recv��:%s"),szRecv);
				}
			}// end if break; 
		}// end switch 
	}//end while 

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//	�����������Socket����
bool CClient::ConnetToServer( SOCKET *pSocket, CString strServer, int nPort )
{
	struct sockaddr_in ServerAddress;
	struct hostent *Server;

	// ����SOCKET
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket) 
	{
		TRACE("���󣺳�ʼ��Socketʧ�ܣ�������Ϣ��%d\n", WSAGetLastError());
		return false;
	}

	// ���ɵ�ַ��Ϣ
	Server = gethostbyname(strServer.GetString());
	if (Server == NULL) 
	{
		closesocket(*pSocket);
		TRACE("������Ч�ķ�������ַ.\n");
		return false; 
	}

	
	ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr, 
		       (char *)Server->h_addr,
		        Server->h_length);

	ServerAddress.sin_port = htons(m_nPort);

	// ��ʼ���ӷ�����
	if (SOCKET_ERROR == connect(*pSocket, reinterpret_cast<const struct sockaddr *>(&ServerAddress),sizeof(ServerAddress))) 
	{
		closesocket(*pSocket);
		TRACE("����������������ʧ�ܣ�\n");
		return false; 
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// ��ʼ��WinSock 2.2
bool CClient::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		ShowMessage(_T("��ʼ��WinSock 2.2ʧ�ܣ�\n"));
		return false; // ����
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// ��ʼ����
bool CClient::Start()
{
	// ����ϵͳ�˳����¼�֪ͨ
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// ���������߳�
	DWORD nThreadID;
	THREADPARAMS_Client* pThreadParams = new THREADPARAMS_Client;
	pThreadParams->pClient = this;

	m_hRecvThread = ::CreateThread(0, 0, _RecvThread, (void *)pThreadParams, 0, &nThreadID);

	m_hConnectionThread = ::CreateThread(0, 0, _ConnectionThread, (void *)pThreadParams, 0, &nThreadID);

	return true;
}

///////////////////////////////////////////////////////////////////////
//	ֹͣ����
void CClient::Stop()
{
	if( m_hShutdownEvent==NULL ) return ;

	SetEvent(m_hShutdownEvent);

	// �ȴ�Connection�߳��˳�
	WaitForSingleObject(m_hConnectionThread, INFINITE);

	// �ر����е�Socket
	for (int i= 0; i< m_nThreads; i++)
	{
		closesocket(m_pParamsWorker[i].sock);
	}
	

	// �ȴ����еĹ������߳��˳�
	WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

	WaitForSingleObject(m_hRecvThread, INFINITE);

	// �����Դ
	CleanUp();

	TRACE("����ֹͣ.\n");
}

//////////////////////////////////////////////////////////////////////
//	�����Դ
void CClient::CleanUp()
{
	if(m_hShutdownEvent==NULL) return;

	RELEASE(m_phWorkerThreads);

	RELEASE_HANDLE(m_hConnectionThread);

	RELEASE_HANDLE(m_hRecvThread);

	RELEASE(m_pParamsWorker);

	RELEASE_HANDLE(m_hShutdownEvent);
}

////////////////////////////////////////////////////////////////////
// ��ñ�����IP��ַ
CString CClient::GetLocalIP()
{
	char hostname[MAX_PATH];
	gethostname(hostname,MAX_PATH);                // ��ñ���������

	struct hostent FAR* lpHostEnt = gethostbyname(hostname);

	if(lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      // ȡ��IP��ַ�б��еĵ�һ��Ϊ���ص�IP

	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);

	m_strLocalIP = CString( inet_ntoa(inAddr) );   // ת���ɱ�׼��IP��ַ��ʽ

	return m_strLocalIP;
}

/////////////////////////////////////////////////////////////////////
// ������������ʾ��Ϣ
void CClient::ShowMessage(const CString strInfo,...)
{
	// ���ݴ���Ĳ�����ʽ���ַ���
	CString   strMessage;
	va_list   arglist;

	va_start(arglist, strInfo);
	strMessage.FormatV(strInfo,arglist);
	va_end(arglist);

	// ������������ʾ
	CMainDlg* pMain = (CMainDlg*)m_pMain;
	if( m_pMain!=NULL )
	{
		pMain->AddInformation(strMessage);
	}
}
