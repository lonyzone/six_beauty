// adoLeakCheck.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "adoLeakCheck.h"
#include "string"

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

//ADO �����
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;
//Ч������
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	InitInstance();
	ConstructOutput();
	_CommandPtr						m_DBCommand;						//�������
	_RecordsetPtr					m_pRecordset;						//��¼������
	_ConnectionPtr					m_pConnection;						//���ݿ����
	EfficacyResult(m_DBCommand.CreateInstance(__uuidof(Command)));
//	EfficacyResult(m_pRecordset.CreateInstance(__uuidof(Recordset)));
	EfficacyResult(m_pConnection.CreateInstance(__uuidof(Connection)));

	//open
	CString m_strConnect;
	m_strConnect.Format("Provider=SQLOLEDB.1;Password=sanyue9394;Persist Security Info=True;User ID=sixbeauty;Initial Catalog=LK78TestDB;Data Source=192.168.3.214,1433;");
	HRESULT hr = m_pConnection->Open(_bstr_t(m_strConnect),L"",L"",adModeUnknown);

	m_DBCommand->CommandType=adCmdStoredProc;
	m_DBCommand->ActiveConnection=m_pConnection;
	
	char roomName[64]={"���ͷ�"};
	ConstructOutput();
	for(int i=0;i<100000;++i)
	{
		_ParameterPtr param;
		param = m_DBCommand->CreateParameter(_TEXT("@serverID"),adInteger,adParamInput,sizeof(long),_variant_t(7794L));
		m_DBCommand->Parameters->Append(param);
		param = m_DBCommand->CreateParameter(_TEXT("@kindID"),adInteger,adParamInput,sizeof(long),_variant_t(93L));
		m_DBCommand->Parameters->Append(param);
		
		param = m_DBCommand->CreateParameter(_TEXT("@RoomID"),adVarChar,adParamInput,sizeof(roomName),_variant_t(roomName));
		m_DBCommand->Parameters->Append(param);

	//	m_DBCommand->CommandText=_bstr_t("GSP_GP_LoadGameRoomConfig");
		m_DBCommand->CommandText=_bstr_t("GSP_GP_UpdateGameRoomConfig");

		m_DBCommand->Execute(NULL, NULL, adExecuteNoRecords);
		/*
		m_pRecordset = m_DBCommand->Execute(NULL, NULL, adCmdStoredProc);
		_variant_t vtValue;
//		vtValue = m_pRecordset->GetCollect("KindID");
		vtValue = m_pRecordset->Fields->GetItem("kindID")->Value;
		unsigned int kindID = vtValue.ulVal;
		//vtValue = m_pRecordset->GetCollect("RoomName");
		vtValue = m_pRecordset->Fields->GetItem("RoomName")->Value;
		std::string str = (_bstr_t)vtValue.bstrVal;
		//	CString execMsg;
		//	execMsg.Format("��ѯ�����kindID %d , roomName %s ",kindID,str.c_str());
		//	std::cout<<execMsg<<std::endl;
		//	AfxMessageBox(execMsg);
		if (m_pRecordset->GetState()==adStateClosed)
		{
			m_pRecordset->Close();
			m_pRecordset.Release();
		}
		*/
		long lParameterCount=m_DBCommand->Parameters->Count;
		if (lParameterCount>0L)
		{
			for (long i=lParameterCount;i>0;i--)
			{
				m_DBCommand->Parameters->Delete(i-1);
			}
		}
	}
	ConstructOutput();

	if(m_pConnection->State)
	{
		m_pConnection->Close();
	}

	m_DBCommand.Release();
	m_pConnection.Release();
	if(m_pRecordset != NULL)
		m_pRecordset.Release();

	unInitInstance();
	return 0;
}
