
#include "Python.h" 
#include <iostream>
#include <string> 
#include <Windows.h>

using namespace std;

//���ĵ����ݶ�������Ҫ���б����ת�������Ϊת��������wchar_t to char
char *w2c(char *pcstr,const wchar_t *pwstr, int len)
{
	int nlength=wcslen(pwstr);
	//��ȡת����ĳ���
	int nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL,0,NULL, NULL ); 
	if(nbytes > len)   nbytes=len;
	// ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
	WideCharToMultiByte( 0,0, pwstr, nlength,   pcstr, nbytes, NULL,   NULL );
	return pcstr ;
}


int main()
{
	Py_Initialize(); 
	// ����ʼ���Ƿ�ɹ�  
	if ( !Py_IsInitialized() ) 
	{ 
		cout<<"��ʼ��pythonʧ��"<<endl;
		return -1; 
	}
	PyRun_SimpleString("import sys"); 
	PyRun_SimpleString("sys.path.append('./')");


	PyObject *pModule = PyImport_ImportModule("pyExcel");

	//������ģ��
	PyObject* pClassComputer = PyObject_GetAttrString(pModule,"pyExcel");

	//��ȡExcel�ļ�����index
	string fileName="1.xlsx";
	int indexNum=0;
	//cout<<"������excel���ļ������������ִ���׺����";
	//cin>>fileName;
	//cout<<endl<<"������excelҪ�򿪵�Index:";
	//cin>>indexNum;
	//cout<<endl<<"========================================="<<endl;

	PyObject* pArg = PyTuple_New(2);

	PyTuple_SetItem(pArg, 0, Py_BuildValue("s",fileName.c_str()));
	PyTuple_SetItem(pArg, 1, Py_BuildValue("i",indexNum));

	PyObject* pClass=PyEval_CallObject(pClassComputer,pArg);

	//��ȡpyExcel�еĸ��ֺ���;
	PyObject* pGetCell=PyObject_GetAttrString(pClass,"getCell");
	PyObject* pGetRows=PyObject_GetAttrString(pClass,"getRows");
	PyObject* pGetCols=PyObject_GetAttrString(pClass,"getCols");
	PyObject* pCellType=PyObject_GetAttrString(pClass,"cellType");

	if (!pGetCell || !pGetCols || !pGetRows || !pCellType ||
		!PyCallable_Check(pGetCell) || !PyCallable_Check(pGetCols) ||
		!PyCallable_Check(pGetRows) || !PyCallable_Check(pCellType) )
	{
		cout<<"��ȡ���еĺ���ʧ�ܣ�"<<endl;
		return -1;
	}

	PyObject *pValue=PyObject_CallFunction(pGetCell, "i,i",0,0);		//ִ�и�ʵ���ĳ�Ա����
	/*
	const wchar_t *t;
	PyArg_Parse(pValue,"s",&t);
	char *pcstr = (char *)malloc(sizeof(char)*(2 * wcslen(t)+1));
	memset(pcstr , 0 , 2 * wcslen(t)+1 );
	w2c(pcstr,t,2 * wcslen(t)+1);
	cout<<"result:"<<pcstr<<endl;
	cout<<strlen(pcstr)<<endl;
	free(pcstr);
	*/

	char *t;
	PyArg_Parse(pValue,"s",&t);
	wcout<<t<<endl;

	Py_Finalize();
	return 0;	
}
