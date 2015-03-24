
#include "Python.h" 
#include <iostream>
#include <string>
#include <codecvt>
using namespace std;

void printDict(PyObject* obj) {
	if (!PyDict_Check(obj))
		return;
	PyObject *k, *keys;
	keys = PyDict_Keys(obj);
	for (int i = 0; i < PyList_GET_SIZE(keys); i++) {
		k = PyList_GET_ITEM(keys, i);
		char* c_name = PyBytes_AsString(k);
		printf("%s\n", c_name);
	}
}


int main(void)
{
	Py_Initialize(); 
	// ����ʼ���Ƿ�ɹ�  
	if ( !Py_IsInitialized() ) 
	{ 
		return -1; 
	} 
	
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	
	std::wstring str = L"pyCalculate"; 
	/*	
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv; 

	std::string  sr= conv.to_bytes(str); 
	//����ģ��
	PyObject* pModule = PyImport_ImportModule(sr.c_str());
	*/
	PyObject* pName = PyUnicode_FromUnicode(str.c_str(),str.size());
	//����ģ��
	PyObject* pModule = PyImport_Import(pName);

	PyObject* calFunc = PyObject_GetAttrString(pModule,"calculate");

	// ����Python����
	PyObject* pValue=PyObject_CallFunction(calFunc, "s","1+2*3-3/2");
	cout<<PyFloat_AS_DOUBLE(pValue)<<endl;

	//��ʾ��������
	/*PyObject* pClassComputer = PyDict_GetItemString(pDict,"computer");
	PyObject* pInstanceComputer = PyInstanceMethod_New(pClassComputer); 

	if(!PyInstanceMethod_Check(pInstanceComputer))
	{
		cout<<"��ʼ����ʧ��!"<<endl;
		return -1;
	}*/
	/*PyObject* pValue=PyObject_GC_New(pInstanceComputer, "calculate","s", "1+2*3-3/2");
	cout<<PyFloat_AS_DOUBLE(pValue)<<endl;*/
//	Py_DECREF(pInstanceComputer);

	Py_Finalize();

	return 0;
}

	

