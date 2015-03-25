﻿// ExcelTest.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <Windows.h>
#include"libxl.h"
#pragma comment (lib,"libxl.lib")
#include"ExcelTest.h"
using namespace std;
using namespace libxl;

#define DLLCLASS_EXPORTS
//中文的内容读出来后要进行编码的转换，这个为转换函数：wchar_t to char
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len)
{
	int nlength=wcslen(pwstr);
	//获取转换后的长度
	int nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL,0,NULL, NULL ); 
	if(nbytes>len)   nbytes=len;
	// 通过以上得到的结果，转换unicode 字符为ascii 字符
	WideCharToMultiByte( 0,0, pwstr, nlength,   pcstr, nbytes, NULL,   NULL );
	return pcstr ;
}

void myDllFunc(char *destStr)
{
	libxl::IBookT<wchar_t>* book;
	bool retLoad;
	int i=0;
	cin>> i;
	if(i!=0)
	{
		book = xlCreateBookW();
		retLoad=book->load(L"E:\\代码\\ExcelTest\\Debug\\3.xls");
	}
	else
	{
		book = xlCreateXMLBookW();
		retLoad=book->load(L"E:\\代码\\ExcelTest\\Debug\\1.xlsx");
	}

	if(retLoad)
	{
		libxl::Sheet * sheet = book->getSheet(0);
		if(sheet){
			libxl::CellType celltype = sheet->cellType(0,0);
			switch(celltype)
			{
			case CELLTYPE_NUMBER:
				{
					double intmy =sheet->readNum(0,0);
					char csNum[23]={};
					sprintf(csNum,"num:%ld",intmy);
					cout<<csNum<<endl;
					break;
				}
			case CELLTYPE_STRING:
				{
					cout<<"true\n";
					const wchar_t * t = sheet->readStr(0,0);
					cout<<t<<endl;
					char *pcstr = (char *)malloc(sizeof(char)*(2 * wcslen(t)+1));
					memset(pcstr , 0 , 2 * wcslen(t)+1 );
					w2c(pcstr,t,2 * wcslen(t)+1);
					cout<<"result:"<<pcstr<<endl;
					cout<<strlen(pcstr)<<endl;
					memcpy(destStr,pcstr,strlen(pcstr));
					free(pcstr);
					break;
				}
			case libxl::CELLTYPE_EMPTY:
				cout<<"true\n";
				break;
			}
		}		
		cout<<"flag\n";
		cout<<sheet->lastRow()<<endl;
		cout<<sheet->lastCol()<<endl;
	}
}

int main()
{
	char myy[23]={};
	while(1)
		myDllFunc(myy);
	cout<<myy<<endl;
}