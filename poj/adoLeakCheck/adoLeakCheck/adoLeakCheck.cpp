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
	_CommandPtr						m_DBCommand;						//�������
	_RecordsetPtr					m_pRecordset;						//��¼������
	_ConnectionPtr					m_pConnection;						//���ݿ����
	EfficacyResult(m_DBCommand.CreateInstance(__uuidof(Command)));
	EfficacyResult(m_pRecordset.CreateInstance(__uuidof(Recordset)));
	EfficacyResult(m_pConnection.CreateInstance(__uuidof(Connection)));

	//open
	CString m_strConnect;
	m_strConnect.Format("Provider=SQLOLEDB.1;Password=sanyue9394;Persist Security Info=True;User ID=sixbeauty;Initial Catalog=LK78TestDB;Data Source=192.168.3.214,1433;");
//	m_strConnect.Format("driver={SQL Server};UserID=sixbeauty;Password=sanyue9394;Server=192.168.3.214,1433;DATABASE=LK78TestDB");
	HRESULT hr = m_pConnection->Open(_bstr_t(m_strConnect),L"",L"",adModeUnknown);
	if (FAILED(hr))
	{
		_com_error e(hr);
		AfxMessageBox(e.ErrorMessage());
		return -1;
	}
	ConstructOutput();
	_bstr_t bstrSQL("SELECT * from GameRoomList where serverID =7794");
	for(int i=0;i<100;++i)
	{
		try
		{
			HRESULT hr = m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
			if (FAILED(hr))
			{
				_com_error e(hr);
				AfxMessageBox(e.ErrorMessage());
				return -1;
			}
			_variant_t vstr, vuint;
			while (!m_pRecordset->EndOfFile)
			{
				vstr = m_pRecordset->GetCollect("RoomName");
				vuint = m_pRecordset->GetCollect("KindID");

			//	std::string str = (_bstr_t)vstr.bstrVal;
				unsigned int uint = vuint.ulVal;
				//CString valMsg;
				//valMsg.Format("��ѯ������kindID:%d,��������%s",uint,str.c_str());
				//std::cout<<valMsg<<std::endl;
				m_pRecordset->MoveNext();
			}
			m_pRecordset->Close();
		}
		catch (_com_error & ComError)
		{
			AfxMessageBox(ComError.Description());
			return -1;
		}
	}
	ConstructOutput();
	if(m_pConnection->State)
	{
		m_pConnection->Close();
	}

	unInitInstance();
	return 0;
}
