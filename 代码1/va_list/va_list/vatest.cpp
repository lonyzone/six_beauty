#include<iostream>
#include<stdarg.h>
using namespace std;

void  LogInfo(char *szFormat,...)
{
	char szBuf[4096]={};
	va_list args;							//��һ��
	va_start(args,szFormat);				//�ڶ���
	_vsnprintf(szBuf,4096,szFormat,args);	//������

	//��������ַ���  szBuf  �������������־�ļ���ֱ�Ӵ�ӡ��Ϣ
	cout<<szBuf<<endl;
	//********************************************************

	va_end(args);							//���Ĳ�

	return ;
}

int main()
{
	LogInfo("�յ����ط�������ת���������:%d,������:%d,������Ϣ",1,2,"hahahaha");

	//ƴ����־�ַ���
	char logStr[4096]={};
	sprintf(logStr,"�յ����ط�������ת���������:%d,������:%d,������Ϣ",1,2,"hahahaha");
	//���
	cout<<logStr<<endl;
	//********************************************************


	return 0;
}