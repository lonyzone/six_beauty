// useAdo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DataBase.h"
#include "useAdo.h"
#include "debug.h"

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

void myRun(CAdoDataBase *pDataBase)
{
	try
	{
		pDataBase->ClearAllParameters();
		pDataBase->SetSPName("GSP_GP_LoadGameRoomConfig");
		pDataBase->AddParamter(_TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t(0L));
		pDataBase->AddParamter(_TEXT("@serverID"),adParamInput,adInteger,sizeof(long),_variant_t(7794L));
		pDataBase->ExecuteCommand(true);

		long lRetValue = pDataBase->GetReturnValue();
		long kindID;
		char roomName[64]={0};
		pDataBase->GetFieldValue(_TEXT("KindID"),kindID);
		pDataBase->GetFieldValue(_TEXT("RoomName"),roomName,sizeof(roomName));
		pDataBase->CloseRecordset();
	//	CString valMsg;
	//	valMsg.Format("��ѯ������kindID:%d,��������%s",kindID,roomName);
	//	std::cout<<valMsg<<std::endl;
	}
	catch (IADOError* pIADOError)
	{
		CString errmsg;
		errmsg.Format(_T("���ݿ���������쳣:%s"),pIADOError->GetErrorDescribe());
		AfxMessageBox(errmsg);
	}

}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	InitInstance();

	CAdoDataBase *pDataBase = new CAdoDataBase();
	pDataBase->SetConnectionInfo("192.168.3.214",1433,"LK78TestDB","sixbeauty","sanyue9394");
	pDataBase->OpenConnection();
	ConstructOutput();
	for(int i=0;i<100;++i)
		myRun(pDataBase);
	delete pDataBase;
	unInitInstance();
	ConstructOutput();

	return 0;
}
