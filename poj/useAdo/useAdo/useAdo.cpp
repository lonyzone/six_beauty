// useAdo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DataBase.h"
#include "useAdo.h"

bool InitInstance()
{
	AfxEnableControlContainer();
	//��ʼ�� com
	CoInitialize(NULL);
	//��ʼ�� OLE DLLs
	if(!AfxOleInit())
	{
		AfxMessageBox("��ʼ��OLE DLLsʧ�ܣ�");
		return false;
	}
	return true;
}

bool unInitInstance()
{
	AfxOleTerm();
	CoUninitialize();
	return true;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	InitInstance();

	CAdoDataBase *pDataBase = new CAdoDataBase();
	pDataBase->SetConnectionInfo("192.168.3.214",1433,"LK78TestDB","sixbeauty","sanyue9394");
	pDataBase->OpenConnection();

	delete pDataBase;
	unInitInstance();
	return 0;
}
