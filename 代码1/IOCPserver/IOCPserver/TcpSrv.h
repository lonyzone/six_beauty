#ifndef _TCPSRV_H
#define _TCPSRV_H

#include "StdAfx.h"
#include<vector>
#include<list>
#include <winsock2.h>
#include <MSWSock.h>
#pragma comment(lib,"ws2_32.lib")
#define SOCK_BUF_SIZE   8192
#define SERVERPORT	 65001
using namespace std;
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
	OVERLAPPED	s_overlapped;		//ϵͳ���첽IO��Ϣ�ṹ��
	SOCKET	s_socket;				//����I/O������ʹ�õ�socket
	WSABUF	s_databuf;				//�洢���ݻ��������������ص��������ݲ���
	char	s_buffer[SOCK_BUF_SIZE];//��ӦWSABUF��Ļ�����
	OPERATION_TYPE	s_optype;				//�����������
	//��ʼ��
	_per_io_data()
	{
		memset(&s_overlapped,0,sizeof(OVERLAPPED));
		memset(&s_buffer,0,SOCK_BUF_SIZE);
		s_socket=INVALID_SOCKET;
		s_databuf.len=0;
		s_databuf.buf=s_buffer;
		s_optype=NULL_TYPE;
	}
	~_per_io_data()
	{
		if(s_socket!=INVALID_SOCKET)
		{
			closesocket(s_socket);
			s_socket=INVALID_SOCKET;
		}
	}
	void ResetBuffer()
	{
		memset(&s_buffer,0,SOCK_BUF_SIZE);
	}
}per_io_data,*p_per_io_data;

//���嵥������ݽṹ�壬ÿ�����������Ӧһ��socket��ÿ��socket���Զ�Ӧ���I/O����
typedef struct _per_socket_data
{
	SOCKET	s_socket;		//��Ӧ�ͻ��˵��׽���
	SOCKADDR_IN	s_clientaddr;	//��Ӧ�ͻ��˵ĵ�ַ
	vector<p_per_io_data>	s_vtiodata;	//socket��I/O���������ݽṹ��
	//����listensocket������socket,��ŵ���acceptIO����
	//����clientsocket���ͻ��ˣ�,���recv��send��IO����
	_per_socket_data()
	{
		s_socket=INVALID_SOCKET;
		memset(&s_clientaddr,0,sizeof(s_clientaddr));
	}
	~_per_socket_data()
	{
		if(s_socket!=INVALID_SOCKET)
		{
			//ѭ��ɾ���ͻ���
			for(vector<p_per_io_data>::iterator iter=s_vtiodata.begin();iter!=s_vtiodata.end();iter++)
			{
				delete *iter;
				iter=s_vtiodata.erase(iter);
			}
			//�ر�socket
			closesocket(s_socket);
		}
		for(vector<p_per_io_data>::iterator iter=s_vtiodata.begin();iter!=s_vtiodata.end();)
		{
			delete *iter;				//��һ�ν�Ӧ��ȡ�õ�I/O�ṹ���ַ���ڶ���ȡ�ýṹ��
			iter=s_vtiodata.erase(iter);
		}
	}//~_per_socket_data;
	p_per_io_data add_new_iodata()
	{
		p_per_io_data newiodata=new per_io_data;
		s_vtiodata.push_back(newiodata);
		return newiodata;

	}
	void dele_iodata(p_per_io_data deledata)
	{
		for(vector<p_per_io_data>::iterator iter=s_vtiodata.begin();iter!=s_vtiodata.end();iter++)
		{
			if(*iter==deledata)
			{
				delete *iter;
				iter=s_vtiodata.erase(iter);
			}
		}
	}
}per_socket_data,*p_per_socket_data;	

class TcpSrv
{
private:
	CRITICAL_SECTION m_cslock;				//�ٽ���
	HANDLE m_EndEvent;						//�����¼�
	HANDLE m_completionport;				//��ɶ˿ھ��
	vector<per_socket_data*> lstclntdata;	//�ͻ��˵�socket���������ͷ��ڴ�
	int m_nThreads;							//�����̵߳ĸ���
	HANDLE *m_workthhdle;					//�����߳̾��
	per_socket_data* m_socketdata;				//����socketdata
	LPFN_ACCEPTEX	m_lpAcceptEx;				//acceptex������ָ��
	LPFN_GETACCEPTEXSOCKADDRS m_lpGetAcceptExSockAddrs;		//GetAcceptSockAddrs������ָ��
	struct sockaddr_in	m_localsockaddr;		//���ص�ַ
	TcpSrv*	m_this;								//���������
public:
	//���캯��
	TcpSrv():
	  m_completionport(INVALID_HANDLE_VALUE),
		  m_workthhdle(NULL),
		  m_nThreads(0),
		  lstclntdata(NULL),
		  m_lpAcceptEx(NULL),
		  m_lpGetAcceptExSockAddrs(NULL)
	  {

	  }
	  //��������
	 	
	  //����ʼ׼�������ض�̬�⣬������ɶ˿ڣ������̳߳�
	  bool setout();							
	  //��ʼ������socket
	  bool initlistensocket();		
	  //��������
	  void startup()
	  {
		  setout();
		  initlistensocket();
	  }
	  //�رշ�����	
	  void closeIocp();
	  //Ͷ���첽accept����
	  bool postaccept(p_per_io_data persockdata);
	  //Ͷ���첽recv����
	  bool postrecv(per_io_data* perIodata);
	  //�������߳�
	  static DWORD WINAPI WorkThread(LPVOID lParam);
	  //workthread�߳�accept������
	  bool _doAccept(per_socket_data* psockdata,per_io_data* piodata);
	  //workthread�߳�recv������
	  bool _doRecv(per_socket_data* psockdata,per_io_data* piodata);
	  //workthread�߳�send������
	  //	bool _doSend(per_socket_data* psockdata,per_io_data* piodata);
	  //�ͷ�����һ���ͻ��˵���Դ
	  void endconnect(per_socket_data* ppsocketdata);
};

#endif