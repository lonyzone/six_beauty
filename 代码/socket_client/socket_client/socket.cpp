#include<iostream>
#include<WinSock2.h>
#include<string>
#include<time.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

const USHORT PORT = 12345;
const std::string ipaddr= "192.168.123.1";

int main()
{
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	sockaddr_in serverAddr;
	serverAddr.sin_addr.s_addr = inet_addr(ipaddr.c_str());
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	SOCKET acceptSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	if(acceptSock == INVALID_SOCKET )
	{
		cout<<"create Socket Error"<<endl;
		return -1;
	}

	if(SOCKET_ERROR == connect(acceptSock,reinterpret_cast<const struct sockaddr *>(&serverAddr),sizeof(serverAddr)))
	{
		closesocket(acceptSock);
		cout<<"����������������ʧ�ܣ�\n"<<endl;
		return -1; 
	}

	int nByteSend=0;
	char bufSend[1024] = {0};

	memset(bufSend,0,sizeof(bufSend));
	lstrcpyn(bufSend,"RecvThread",sizeof(bufSend));
	nByteSend = send(acceptSock,bufSend,strlen(bufSend)+1,0);
	if(nByteSend < 0)
	{
		cout<<"send fail!"<<endl;
	}
	
	/*
	cin>>nByteSend;

	memset(bufSend,0,sizeof(bufSend));
	lstrcpyn(bufSend,"���͵ڶ���",sizeof(bufSend));
	nByteSend = send(acceptSock,bufSend,strlen(bufSend)+1,0);
	if(nByteSend < 0)
	{
		cout<<"send fail!"<<endl;
	}
	*/
	int nByteRecv=0;
	char szRecv[1024] = {0};

	/*
	while(true)
	{
		ZeroMemory(szRecv,sizeof(szRecv)/sizeof(szRecv[0]));
		int nBytesRecv = recv(acceptSock,szRecv,sizeof(szRecv),0);
		if(nBytesRecv > 0)
		{
			cout<<"��Recv��:"<<szRecv<<endl;
		}
	}
	*/
	SOCKADDR_IN    addrSender;
	int addrSize = sizeof(SOCKADDR_IN);

	struct fd_set fds; 
	struct timeval timeout={3,0}; //select�ȴ�3�룬3����ѯ��Ҫ����������0 
	while(true)
	{ 
		FD_ZERO(&fds); //ÿ��ѭ����Ҫ��ռ��ϣ������ܼ���������仯 
		FD_SET(acceptSock,&fds); //��������� 
		int nrecv = select(0,&fds,NULL,NULL,&timeout);
		switch(nrecv) //selectʹ�� 
		{ 
		case -1: 
			{
				//select����
				return false;
			}
		case 0: 
			{
				// �����û���ֹͣ�¼�
				int nContinue=0;
				cout<<"�Ƿ����(0/1):";
				cin>>nContinue;
				if(nContinue==0)
				{
					cout<<"���յ��û�ֹͣ����.\n";
					return 0;
				}	
				break;
			}
		default: 
			if(FD_ISSET(acceptSock,&fds)) //����sock�Ƿ�ɶ������Ƿ������������� 
			{ 
				ZeroMemory(szRecv,sizeof(szRecv)/sizeof(szRecv[0]));
				int nBytesRecv = recv(acceptSock,szRecv,sizeof(szRecv),0);
				if(nBytesRecv > 0)
				{
					cout<<"��Recv��:"<<szRecv<<endl;
				}else
				{
					int nwrite = select(0,NULL,&fds,NULL,&timeout);
					if(nwrite == -1)
					{
						cout<<"д��ʧ��!�ر�����..."<<endl;
						return -1;
					}
					else if(nBytesRecv ==0 && nwrite ==1)
					{
						cout<<"��ȡ��0byte�����ݣ���socket�ӿ���д��..."<<endl;
					}
				}
			}// end if break; 
		}// end switch 
	}//end while 

	closesocket(acceptSock);
	WSACleanup();

	return 0;
};
