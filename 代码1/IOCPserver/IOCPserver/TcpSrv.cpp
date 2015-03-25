#include"StdAfx.h"
#include"TcpSrv.h"
#include<iostream>
using namespace std;
//��ʼ��,���ؿ⣬�����˿ڣ��̳߳أ������ܷ��ⲿ�ֿ��Էֳ�3�������������Ƿ���ֵ��Ϊbool����setout������return && && ;��̫�鷳��~_~)
bool TcpSrv::setout()
{
	//����socket��
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		cerr<<"��socket��̬���ӿ����..."<<endl;
		return false;  
	}
	//��ʼ�����������˳��¼�
	InitializeCriticalSection(&m_cslock);
	m_EndEvent=CreateEvent(NULL,true,false,NULL);
	//������ɶ˿�
	if((m_completionport=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0))=NULL)
	{
		cerr<<"�����˿�ʧ��..."<<endl;
		return false;
	}

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
		if((m_workthhdle[i]=CreateThread(NULL,0,WorkThread,this,0,NULL))==NULL)
		{
			cerr<<"�̳߳ش���ʧ��..."<<endl;
			return false;  
		}
	}
	return true;
}

//��ʼ��listensocket
bool TcpSrv::initlistensocket()
{
	// �������ڼ�����Socket����Ϣ
	m_socketdata=new per_socket_data;
	//��������socket
	if((m_socketdata->s_socket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED))==INVALID_SOCKET)
	{
		cerr<<"����socket��ʼ��ʧ��..."<<endl;
		return false;
	}
	//��listensocket�󶨵���ɶ˿���
	if(NULL==CreateIoCompletionPort((HANDLE)m_socketdata->s_socket,m_completionport,(DWORD)m_socketdata,0))
	{
		cerr<<"��listensocket����ɶ˿�ʧ��...."<<endl;
		if(m_socketdata->s_socket!=INVALID_SOCKET)
			closesocket(m_socketdata->s_socket);
		return false;
	}
	//���ñ���addr
	memset(&m_localsockaddr,0,sizeof(m_localsockaddr));
	m_localsockaddr.sin_family=AF_INET;
	m_localsockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	m_localsockaddr.sin_port=htons(SERVERPORT);
	//�󶨼���socket�Ͷ˿�
	if(bind(m_socketdata->s_socket,(SOCKADDR*)&m_localsockaddr,sizeof(m_localsockaddr)!=0))
	{
		cerr<<"����socket�Ͷ˿ڰ�ʧ��..."<<endl;
		return false;
	}
	//������ʼ
	if(listen(m_socketdata->s_socket,SOMAXCONN)!=0)
	{
		cerr<<"��������ʧ��..."<<endl;
		return false;
	}

	//��δ��ȡaccpetex�����͵��������Ľϴ�����WSAIoctl��ȡ����ָ��
	GUID guidAcceptEx=WSAID_ACCEPTEX;
	GUID guidGetAcceptExSockaddrs=WSAID_GETACCEPTEXSOCKADDRS;
	DWORD acByte=0;
	if(WSAIoctl(m_socketdata->s_socket,SIO_GET_EXTENSION_FUNCTION_POINTER,&guidAcceptEx,sizeof(guidAcceptEx),
		m_lpAcceptEx,sizeof(m_lpAcceptEx),&acByte,0,0)==0)
	{
		cerr<<"AcceptEx������ȡʧ��..."<<endl;
		return false;
	}
	//�����ȡGetAcceptExSockAddrs�ĵ�ַ
	if(WSAIoctl(m_socketdata->s_socket,SIO_GET_EXTENSION_FUNCTION_POINTER,&guidGetAcceptExSockaddrs,sizeof(guidGetAcceptExSockaddrs),
		&m_lpGetAcceptExSockAddrs,sizeof(m_lpGetAcceptExSockAddrs),&acByte,0,0)==0)
	{
		cerr<<"AcceptEx������ȡʧ��..."<<endl;
		return false;
	}
	//acceptEx����ɶ˿���Ͷ�ݶ������	
	for(int i=0;i<10;i++)
	{
		per_io_data *periodata=m_socketdata->add_new_iodata();
		periodata->s_optype=ACCEPT_TYPE;
		periodata->s_socket=m_socketdata->s_socket;
		if(postaccept(periodata)==false)
		{
			m_socketdata->dele_iodata(periodata);
			return false;
		}
	}
	cout<<"��ʼ���������ɹ�����ʼͶ��accept����..."<<endl;
	return true;
}
void TcpSrv::closeIocp()
{

}
DWORD WINAPI TcpSrv::WorkThread(LPVOID lpParam)
{
	TcpSrv* mythis=(TcpSrv*)lpParam;
	OVERLAPPED	*pOverapped=NULL;
	per_socket_data	*pSocketdata=NULL;
	per_io_data	*pIodata=NULL;
	DWORD	dwBytesTransfered=0;

	//ѭ����������
	while(WAIT_OBJECT_0!=WaitForSingleObject(mythis->m_EndEvent,0))
	{
		if(GetQueuedCompletionStatus(
			mythis->m_completionport,
			&dwBytesTransfered,
			(PULONG_PTR)pSocketdata,
			&pOverapped,
			INFINITE
			)==0)
		{
			cerr<<"�����߳�GetQueuedCompletionStatus����..."<<endl;
			continue;
		}//if
		pIodata=(per_io_data*)CONTAINING_RECORD(pOverapped,per_io_data,s_overlapped);
		if(dwBytesTransfered==0 && ( pIodata->s_optype==RECV_TYPE || pIodata->s_optype==SEND_TYPE))
		{
			//�ͷŵ��߿ͻ��˵���Դ
			mythis->endconnect(pSocketdata);
			continue;
		}//if
		switch(pIodata->s_optype)
		{
		case ACCEPT_TYPE:
			mythis->_doAccept(pSocketdata,pIodata);
			break;
		case RECV_TYPE:
			mythis->_doRecv(pSocketdata,pIodata);
			break;
		case SEND_TYPE:
			//		mythis->_doSend(pSocketdata,pIodata);
			break;
		default:
			cerr<<"workthread�����쳣..."<<endl;
			break;
		}//switch
	}//while
	return 0;
}
//�ͷ�һ���ͻ��˵���Դ
void TcpSrv::endconnect(per_socket_data* ppsocketdata)
{
	EnterCriticalSection(&m_cslock);
	for(vector<per_socket_data*>::iterator iter=lstclntdata.begin();iter!=lstclntdata.end();iter++)
	{
		if((*iter)==ppsocketdata)
		{
			delete *iter;
			lstclntdata.erase(iter);
		}
		break;
	}
	LeaveCriticalSection(&m_cslock);
}
//Ͷ��recv����
bool TcpSrv::postrecv(per_io_data* piodata)
{
	//WSARecv�Ĳ���׼��
	DWORD dwSendBytes=0;
	DWORD dwInfo=0;
	WSABUF *databuf=&piodata->s_databuf;
	piodata->ResetBuffer();
	piodata->s_optype=RECV_TYPE;
	//Ͷ��WSARecv����
	if(WSARecv(piodata->s_socket,databuf,1,&dwSendBytes,&dwInfo,&piodata->s_overlapped,NULL)==SOCKET_ERROR)
	{
		cerr<<"Ͷ��recv����ʧ��..."<<endl;
		return false;
	}
	return true;
}
//Ͷ��accept����
bool TcpSrv::postaccept(per_io_data* piodata)
{
	//ΪacceptEx׼��socket��acceptEx��accept��Ч���ص㣩
	if((piodata->s_socket=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED))==INVALID_SOCKET)
	{
		cerr<<"ΪacceptEx�����׽���ʧ��..."<<endl;
		return false;
	}
	//ΪacceptEx׼������
	DWORD bytereceived;
	WSABUF* databuf=&piodata->s_databuf;

	//Ͷ��accpetEx����
	if(m_lpAcceptEx(m_socketdata->s_socket,piodata->s_socket,databuf->buf, databuf->len - ((sizeof(SOCKADDR_IN)+16)*2),   
		sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &bytereceived, &piodata->s_overlapped)==false)
	{
		cerr<<"Ͷ��acceptExʧ��..."<<endl;
		return false;
	}
	return true;
}
//�����߳�workthread�ж�accpet_type�Ĳ���
bool TcpSrv::_doAccept(per_socket_data* plistsockdata,per_io_data* piodata)
{
	//ͨ��GetAcceptExSockAddrs()��õ�ַ��Ϣ
	SOCKADDR_IN *LocalAddr=NULL;
	SOCKADDR_IN *ClientAddr=NULL;
	int localLen=sizeof(SOCKADDR_IN),clientLen=sizeof(SOCKADDR_IN);
	m_lpGetAcceptExSockAddrs(piodata->s_databuf.buf,piodata->s_databuf.len-((sizeof(SOCKADDR_IN)+16)*2),sizeof(SOCKADDR_IN)+16,sizeof(SOCKADDR_IN)+16,
		(LPSOCKADDR*)LocalAddr,&localLen,(LPSOCKADDR*)ClientAddr,&clientLen);
	cout<<"�û�:"<<inet_ntoa(ClientAddr->sin_addr)<<"��½������..."<<endl;
	cout<<inet_ntoa(ClientAddr->sin_addr)<<"	:"<<piodata->s_databuf.buf<<endl;
	//����client��socketdata��Ϣ
	per_socket_data* pnew_sockdata=new per_socket_data;
	pnew_sockdata->s_socket=piodata->s_socket;
	memcpy(&pnew_sockdata->s_clientaddr,ClientAddr,sizeof(SOCKADDR_IN));			//����Ҫע�ⲻ��ֱ�Ӹ�ֵ��ָ�븳ֵ������////////////////////
	//��clientsocket����ɶ˿���
	if(NULL==CreateIoCompletionPort((HANDLE)pnew_sockdata->s_socket,m_completionport,(DWORD)pnew_sockdata,0))
	{
		cerr<<"��clientsocket("<<inet_ntoa(ClientAddr->sin_addr)<<")����ɶ˿�ʧ��...."<<endl;
		if(m_socketdata->s_socket!=INVALID_SOCKET)
			closesocket(m_socketdata->s_socket);
		return false;
	}
	//Ϊ���clientͶ�ݵ�һ��recv����
	per_io_data* pnew_iodata=pnew_sockdata->add_new_iodata();
	pnew_iodata->s_optype=RECV_TYPE;
	pnew_iodata->s_socket=pnew_sockdata->s_socket;
	if(postrecv(pnew_iodata)==false)
	{
		cerr<<"Ϊclientsocket("<<inet_ntoa(ClientAddr->sin_addr)<<")Ͷ�ݵ�һ��recv����ʧ��..."<<endl;
		pnew_sockdata->dele_iodata(pnew_iodata);
		return false;
	}
	//���½���clientdata����lstclntdata��
	EnterCriticalSection(&m_cslock);
	lstclntdata.push_back(pnew_sockdata);
	LeaveCriticalSection(&m_cslock);
	//�������ݻ�����������Ͷ��accpetEx����
	piodata->ResetBuffer();
	return postrecv(piodata);
}
//�����߳�workthread�ж�recv�Ĳ���
bool TcpSrv::_doRecv(per_socket_data* psockdata,per_io_data* piodata)
{
	cout<<"�ͻ�("<<inet_ntoa(psockdata->s_clientaddr.sin_addr)<<"):"<<endl<<"	"<<piodata->s_databuf.buf<<endl;
	return postrecv(piodata);
}
