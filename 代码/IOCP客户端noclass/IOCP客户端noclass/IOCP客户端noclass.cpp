// IOCP�ͻ���ģ��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
// IOCP_TCPIP_Socket_Client.cpp

#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <Windows.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib")		// Socket������õĶ�̬���ӿ�

SOCKET sockClient;		// ���ӳɹ�����׽���
HANDLE bufferMutex;		// �����ܻ���ɹ�����ͨ�ŵ��ź������
const int DefaultPort = 12345;

int main()
{
	// ����socket��̬���ӿ�(dll)
	WORD wVersionRequested;
	WSADATA wsaData;	// ��ṹ�����ڽ���Wjndows Socket�Ľṹ��Ϣ��
	wVersionRequested = MAKEWORD( 2, 2 );	// ����2.2�汾��WinSock��
	int err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {	// ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup
		return -1;
	}

	// ����socket������������ʽ�׽��֣������׽��ֺ�sockClient
	sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockClient == INVALID_SOCKET) { 
		printf("Error at socket():%ld\n", WSAGetLastError()); 
		WSACleanup(); 
		return -1; 
	} 

	// ���׽���sockClient��Զ����������
	// int connect( SOCKET s,  const struct sockaddr* name,  int namelen);
	// ��һ����������Ҫ�������Ӳ������׽���
	// �ڶ����������趨����Ҫ���ӵĵ�ַ��Ϣ
	// ��������������ַ�ĳ���
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.3.214");		// ���ػ�·��ַ��127.0.0.1; 
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(DefaultPort);
	while(SOCKET_ERROR == connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR))){
		// �����û�����Ϸ�������Ҫ������
		cout << "����������ʧ�ܣ��Ƿ��������ӣ���Y/N):";
		char choice;
		while(cin >> choice && (!((choice != 'Y' && choice == 'N') || (choice == 'Y' && choice != 'N')))){
			cout << "�����������������:";
			cin.sync();
			cin.clear();
		}
		if (choice == 'Y'){
			continue;
		}
		else{
			cout << "�˳�ϵͳ��...";
			system("pause");
			return 0;
		}
	}
	cin.sync();
	cout << "���ͻ�����׼���������û���ֱ�����������������������Ϣ��\n";

	int nret=send(sockClient, "Hello!", 200, 0);
	if(nret==0)
	{
		printf("��һ�η���ʧ��!\n");
	}
	bufferMutex = CreateSemaphore(NULL, 1, 1, NULL); 

	DWORD WINAPI SendMessageThread(LPVOID IpParameter);

	HANDLE sendThread = CreateThread(NULL, 0, SendMessageThread, NULL, 0, NULL);  



	WaitForSingleObject(sendThread, INFINITE);  // �ȴ��߳̽���
	closesocket(sockClient);
	CloseHandle(sendThread);
	CloseHandle(bufferMutex);
	WSACleanup();	// ��ֹ���׽��ֿ��ʹ��

	printf("End linking...\n");
	printf("\n");
	system("pause");
	return 0;
}


DWORD WINAPI SendMessageThread(LPVOID IpParameter)
{
	while(1){
		string talk;
		getline(cin, talk);
		WaitForSingleObject(bufferMutex, INFINITE);		// P����Դδ��ռ�ã�  
		if("quit" == talk){
			talk.push_back('\0');
			send(sockClient, talk.c_str(), 200, 0);
			break;
		}
		else{
			talk.append("\n");
		}
		printf("\nI Say:(\"quit\"to exit):");
		cout << talk;
		send(sockClient, talk.c_str(), 200, 0);	// ������Ϣ
		ReleaseSemaphore(bufferMutex, 1, NULL);		// V����Դռ����ϣ� 
	}
	return 0;
}




