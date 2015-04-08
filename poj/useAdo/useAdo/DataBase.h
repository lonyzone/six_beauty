#ifndef _ADO_DATABASE_H
#define _ADO_DATABASE_H
#include <ICrsint.h>

//ADO �����
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

//COM ��������
typedef _com_error					CComError;							//COM ����

//���ݿ�������
enum enADOErrorType
{
	ErrorType_Nothing				=0,									//û�д���
	ErrorType_Connect				=1,									//���Ӵ���
	ErrorType_Other					=2,									//��������
};

//���ݿ����ӿ�
struct IADOError 
{
	//��������
	virtual LPCTSTR __cdecl GetErrorDescribe()=NULL;
	//��������
	virtual enADOErrorType __cdecl GetErrorType()=NULL;
};


//ADO ������
class CADOError : public IADOError
{
	//��������
protected:
	enADOErrorType					m_enErrorType;						//�������
	CString							m_strErrorDescribe;					//������Ϣ

	//��������
public:
	//���캯��
	CADOError();
	//��������
	virtual ~CADOError();

	//���ܽӿ�
public:
	//��������
	virtual enADOErrorType __cdecl GetErrorType() { return m_enErrorType; }
	//��������
	virtual LPCTSTR __cdecl GetErrorDescribe() { return m_strErrorDescribe; }

	//���ܺ���
public:
	//���ô���
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����
class CAdoDataBase 
{
	//��Ϣ����
protected:
	CADOError						m_ADOError;							//�������
	CString							m_strConnect;						//�����ַ���
	CString							m_strErrorDescribe;					//������Ϣ

	//״̬����
protected:
	DWORD							m_dwConnectCount;					//���Դ���
	DWORD							m_dwConnectErrorTime;				//����ʱ��
	const DWORD						m_dwResumeConnectCount;				//�ָ�����
	const DWORD						m_dwResumeConnectTime;				//�ָ�ʱ��

	//�ں˱���
protected:
	_CommandPtr						m_DBCommand;						//�������
	_RecordsetPtr					m_DBRecordset;						//��¼������
	_ConnectionPtr					m_DBConnection;						//���ݿ����

	//��������
public:
	//���캯��
	CAdoDataBase();
	//��������
	virtual ~CAdoDataBase();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() {delete this; }
	//�Ƿ���Ч
	bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAdoDataBase))?true:false; }
	
	//����ӿ�
public:
	//������
	virtual bool __cdecl OpenConnection();
	//�رռ�¼
	virtual bool __cdecl CloseRecordset();
	//�ر�����
	virtual bool __cdecl CloseConnection();
	//��������
	virtual bool __cdecl TryConnectAgain(bool bFocusConnect, CComError * pComError);
	//������Ϣ
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szIP, WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass);

	//״̬�ӿ�
public:
	//�Ƿ����Ӵ���
	virtual bool __cdecl IsConnectError();
	//�Ƿ��
	virtual bool __cdecl IsRecordsetOpened();

	//��¼���ӿ�
public:
	//�����ƶ�
	virtual void __cdecl MoveToNext();
	//�Ƶ���ͷ
	virtual void __cdecl MoveToFirst();
	//�Ƿ����
	virtual bool __cdecl IsEndRecordset();
	//��ȡ��Ŀ
	virtual long __cdecl GetRecordCount();
	//��ȡ��С
	virtual long __cdecl GetActualSize(LPCTSTR pszParamName);
	//�󶨶���
	virtual bool __cdecl BindToRecordset(CADORecordBinding * pBind);

	//�ֶνӿ�
public:
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);

	//�������ӿ�
public:
	//���ô洢����
	virtual void __cdecl SetSPName(LPCTSTR pszSpName);
	//�������
	virtual void __cdecl AddParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, ADOCG::DataTypeEnum Type, long lSize, _variant_t & vtValue);
	//ɾ������
	virtual void __cdecl ClearAllParameters();
	//��ò���
	virtual void __cdecl GetParameterValue(LPCTSTR pszParamName, _variant_t & vtValue);
	//��ȡ������ֵ
	virtual long __cdecl GetReturnValue();

	//ִ�нӿ�
public:
	//ִ�����
	virtual bool __cdecl Execute(LPCTSTR pszCommand);
	//ִ������
	virtual bool __cdecl ExecuteCommand(bool bRecordset);

	//�ڲ�����
private:
	//��ȡ����
	LPCTSTR GetComErrorDescribe(CComError & ComError);
	//���ô���
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);
};


#endif
