// asdf.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
using namespace std;
void delPoint(CString &sourStr)
{
	int ptPos=sourStr.Find('.');
	int sourLen=sourStr.GetLength();
	if(ptPos==-1)return;		//û����������

	CString leftStr=sourStr.Right(sourLen-ptPos-1);		//С������������
	CString zeroStr('0',sourLen-ptPos-1);				//����ȫ0��CString

	if(leftStr.Compare(zeroStr)==0)						//С�����ȫΪ0��ض�
	{
		sourStr=sourStr.Left(ptPos);
	}
}
int main()
{
	CString my;
	
	my.Format("101.222");
	delPoint(my);
	cout<<"flag:"<<my<<endl;

	my.Format("101.000");
	delPoint(my);
	cout<<"flag:"<<my<<endl;


	my.Format("101.20");
	delPoint(my);
	cout<<"flag:"<<my<<endl;
	return 0;
}

