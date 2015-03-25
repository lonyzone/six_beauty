// cricital.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
#include <iostream>
using namespace std;
DWORD WINAPI threadFun1(
	LPVOID lpParameter   // �߳�1���̺߳�������
	);
DWORD WINAPI threadFun2(
	LPVOID lpParameter   // �߳�2���̺߳�������
	);

int my1[10]={0};
int my2[10]={0};
CRITICAL_SECTION g_cs;//�ٽ����Ĳ���
///////////////////////////////////////////////////////////////////////
//���ƣ�main//
//���ܣ��������߳�ģ���վ����Ʊϵͳ�������ٽ���ʵ���̵߳�ͬ��//
//////////////////////////////////////////////////////////////////////
void main()
{
	InitializeCriticalSection(&g_cs);//��ʼ���ٽ���
	HANDLE hThread1;
	hThread1=CreateThread(NULL,0,threadFun1,NULL,0,NULL);
	HANDLE hThread2;
	hThread2=CreateThread(NULL,0,threadFun2,NULL,0,NULL);
	CloseHandle(hThread1);//�����߳�1
	CloseHandle(hThread2);//�����ֳ�

	Sleep(4000);//������˯��4�룬ʹ�̺߳�����������
	DeleteCriticalSection(&g_cs);//����ִ����ϣ������ٽ���
}
DWORD WINAPI threadFun1(LPVOID lpParameter)   // �߳�1
{
	while(1)
	{
	//	EnterCriticalSection(&g_cs);
		Sleep(1000);
		printf("+++++++++\n");
	//	LeaveCriticalSection(&g_cs);//�ͷ��ٽ�����Դ
	}
	return 0;
}

DWORD WINAPI threadFun2(LPVOID lpParameter)   // �߳�2
{
	while(1)
	{
	//	EnterCriticalSection(&g_cs);
		Sleep(400);
		printf("---------\n");
	//	LeaveCriticalSection(&g_cs);//�ͷ��ٽ�����Դ
	}
	return 0;
}