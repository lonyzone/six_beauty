/*********************************************************************** 
*�����ʼ�ģ��ͷ�ļ� 
*���Է����ı��͸�����֧�ֶ������һ���ͣ� 
*************************************************************************/  
#pragma once  
//struct sMailInfo //�ʼ���Ϣ  
//{  
//    char*   m_pcUserName;//�û���¼���������  
//    char*   m_pcUserPassWord;//�û���¼���������  
//    char*   m_pcSenderName;//�û�����ʱ��ʾ������  
//    char*   m_pcSender;//�����ߵ������ַ  
//    char*   m_pcReceiver;//�����ߵ������ַ  
//    char*   m_pcTitle;//�������  
//    char*   m_pcBody;//�ʼ��ı�����  
//    char*   m_pcIPAddr;//��������IP  
//    char*   m_pcIPName;//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�  
//    sMailInfo(){memset(this,0,sizeof(sMailInfo));}  
//};  
struct sMailInfo //�ʼ���Ϣ  
{  
	char   m_pcUserName[128];//�û���¼���������  
	char   m_pcUserPassWord[128];//�û���¼���������  
	char   m_pcSenderName[128];//�û�����ʱ��ʾ������  
	char   m_pcSender[128];//�����ߵ������ַ  
	char   m_pcReceiver[128];//�����ߵ������ַ  
	char   m_pcTitle[128];//�������  
	char   m_pcBody[1024*10];//�ʼ��ı�����  
	char   m_pcIPAddr[128];//��������IP  
	char   m_pcIPName[128];//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�  
	sMailInfo(){memset(this,0,sizeof(sMailInfo));}  
}; 
class CSendMail  
{  
public:  
    CSendMail(void);  
    ~CSendMail(void);  
  
public:  
    bool SendMail(sMailInfo &smailInfo);//�����ʼ�����Ҫ�ڷ��͵�ʱ���ʼ���ʼ���Ϣ  
    void AddFilePath(char * pcFilePath);//��Ӹ����ľ���·���������б���  
    void DeleteFilePath(char* pcFilePath);//ɾ������·��������еĻ�  
    void DeleteAllPath(void);//ɾ��ȫ��������·��  
  
protected:  
    void GetFileName(char* fileName,char* filePath);//�Ӹ�����·���л�ȡ�ļ�����  
    void Char2Base64(char* pBuff64,char* pSrcBuff,int iLen);//��char����ת����Base64����  
    bool  CReateSocket(SOCKET &sock);//����socket����  
    bool Logon(SOCKET &sock);//��¼���䣬��Ҫ���з��ʼ�ǰ��׼������  
    int GetFileData(char* FilePath);//���ļ�·����ȡ��������  
  
    bool SendHead(SOCKET &sock);//�����ʼ�ͷ  
    bool SendTextBody(SOCKET &sock);//�����ʼ��ı�����  
    bool SendFileBody(SOCKET &sock);//�����ʼ�����  
    bool SendEnd(SOCKET &sock);//�����ʼ���β  
protected:  
    CList<char*,char*> m_FilePathList;//��¼����·��  
  
    char  m_SendBuff[4096];//���ͻ�����  
    char  m_cReceiveBuff[1024];  
    char* m_pFileBuffer;//ָ�򸽼�����  
    sMailInfo m_sMailInfo;  
};  
