

/************************************************************************ 
* �����ʼ�ģ�� 
*���Է����ı��͸�����֧�ֶ������һ���ͣ� 
*Date:2011-12-01 
************************************************************************/  
#include <stdafx.h>
#include "CSendMail.h"  
#include "winsock2.h"  
#pragma comment(lib,"WSOCK32")  
#pragma comment(lib, "ws2_32")

CSendMail::CSendMail(void)  
{  
	m_pFileBuffer=NULL;  
	memset(m_SendBuff,0,sizeof(m_SendBuff));  
	memset(m_cReceiveBuff,0,sizeof(m_cReceiveBuff));  
}  


CSendMail::~CSendMail(void)  
{  
	DeleteAllPath();  
}  

  
void CSendMail::Char2Base64(char* pBuff64,char* pSrcBuff,int iLen)  
{  
    //1   1   1   1   1   1   1   1  
    // �����pBuff64  �� �����pBuff64+1  
    //         point���ڵ�λ��  
    static char Base64Encode[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//base64��ӳ����ַ���  
    int point;//ÿһ��Դ�ַ���ֵ�λ�ã���ȡ2,4,6����ʼΪ2  
    point=2;  
    int i;  
    int iIndex;//base64�ַ�������  
    char n=0;//��һ��Դ�ַ��Ĳ���ֵ  
    for(i=0;i<iLen;i++)  
    {  
        if(point==2)  
        {  
            iIndex=((*pSrcBuff)>>point)&0x3f;//ȡ��pSrcBuff�ĸ�pointλ  
        }  
        else if (point==4)  
        {  
            iIndex=((*pSrcBuff)>>point)&0xf;//ȡ��pSrcBuff�ĸ�pointλ  
        }  
        else if(point==6)  
        {  
            iIndex=((*pSrcBuff)>>point)&0x3;//ȡ��pSrcBuff�ĸ�pointλ  
        }  
        iIndex+=n;//��pSrcBuff-1�ĵ�point������Base64������  
        *pBuff64++=Base64Encode[iIndex];//��������õ�pBuff64  
        n=((*pSrcBuff)<<(6-point));//����Դ�ַ��еĲ���ֵ  
        n=n&0x3f;//ȷ��n�������λΪ0  
        point+=2;//Դ�ַ��Ĳ��λ������2  
        if(point==8)//������λ��Ϊ8˵��pSrcBuff��6λ�������������һ��������Base64�ַ�������ֱ�������һ��  
        {  
            iIndex=(*pSrcBuff)&0x3f;//��ȡ��6λ�������������  
            *pBuff64++=Base64Encode[iIndex];//  
            n=0;//����ֵΪ0  
            point=2;//���λ����Ϊ2  
        }  
        pSrcBuff++;  
  
    }  
    if(n!=0)  
    {  
        *pBuff64++=Base64Encode[n];  
    }  
    if(iLen%3==2)//���Դ�ַ������Ȳ���3�ı���Ҫ��'='��ȫ  
    {  
        *pBuff64='=';  
    }  
    else if(iLen%3==1)  
    {  
        *pBuff64++='=';  
        *pBuff64='=';  
    }  
}  
  
void CSendMail::AddFilePath(char * pcFilePath)//��Ӹ���·��  
{  
    if(pcFilePath==NULL)  
    {  
        return;  
    }  
    int i;  
    char* temp;  
    for(i=0;i<m_FilePathList.GetCount();i++)  
    {  
        temp=m_FilePathList.GetAt(m_FilePathList.FindIndex(i));  
        if(strcmp(pcFilePath,temp)==0)//����Ѿ����ھͲ����������  
        {  
            return;  
        }  
    }  
    m_FilePathList.AddTail(pcFilePath);  
}  
  
void CSendMail::DeleteFilePath(char* pcFilePath)//ɾ������·��  
{  
    int i;  
    char* temp;  
    for(i=0;i<m_FilePathList.GetCount();i++)  
    {  
        temp=m_FilePathList.GetAt(m_FilePathList.FindIndex(i));  
        if(strcmp(temp,pcFilePath)==0)//�ҵ���ɾ���������û�ҵ�������  
        {  
            m_FilePathList.RemoveAt(m_FilePathList.FindIndex(i));  
            delete[] temp;  
            return;  
        }  
    }  
}  
  
  
void CSendMail::DeleteAllPath(void)  
{  
    m_FilePathList.RemoveAll();  
}  
  
int CSendMail::GetFileData(char* szFilePath)  
{  
	m_pFileBuffer=NULL;  
	if(szFilePath==NULL)  
	{  
		return 0;  
	}  
	CFile f;  
	int len;  


	USES_CONVERSION;  
	if(!f.Open(TEXT(szFilePath),CFile::modeRead|CFile::modeNoTruncate|CFile::typeBinary))  
	{  
		return 0;  
	}  
	len=(int)f.GetLength();  
	m_pFileBuffer=new char[len+1];  
	memset(m_pFileBuffer,0,len+1);  
	f.Read(m_pFileBuffer,len);  
	printf("==========================================================================\n");
	printf("%s\n",m_pFileBuffer);
	printf("==========================================================================\n");

	f.Close();  
	return len;  
}  

void CSendMail::GetFileName(char* fileName,char* filePath)  
{  
	if(filePath==NULL || fileName==NULL)  
	{  
		return;  
	}  
	int i;  
	for(i=0;i<(int)strlen(filePath);i++)  
	{  
		if(filePath[strlen(filePath)-1-i]=='\\')  
		{  
			memcpy(fileName,&filePath[strlen(filePath)-i],i);  
			return;  
		}  
	}  
}  
  
bool CSendMail::CReateSocket(SOCKET &sock)  
{  
    WORD wVersionRequested;  
    WSADATA wsaData;  
    int err;  
    wVersionRequested = MAKEWORD( 2, 2 );  
    err = WSAStartup( wVersionRequested, &wsaData );  
    if ( err != 0 )   
    {  
        return false;  
    }  
    if ( LOBYTE( wsaData.wVersion ) != 2 ||  
        HIBYTE( wsaData.wVersion ) != 2 )  
    {  
        WSACleanup( );  
        return false;   
    }  
    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);  
    if (sock == INVALID_SOCKET)  
    {  
        return false;  
    }  
  
    sockaddr_in servaddr;  
    memset(&servaddr,0,sizeof(sockaddr_in));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(25);//���ʼ�һ�㶼��25�˿�  
    if(m_sMailInfo.m_pcIPName=="")  
    {  
        servaddr.sin_addr.s_addr = inet_addr(m_sMailInfo.m_pcIPAddr);//ֱ��ʹ��IP��ַ  
    }  
    else  
    {  
        struct hostent *hp=gethostbyname(m_sMailInfo.m_pcIPName);//ʹ������  
		if (hp == NULL)
		{
			DWORD dwErrCode =	GetLastError();
			return false;
		}
        servaddr.sin_addr.s_addr=*(int*)(*hp->h_addr_list);  
    }  
  
  
    int ret = connect(sock,(sockaddr*)&servaddr,sizeof(servaddr));//��������  
    if (ret == SOCKET_ERROR)  
    {  
		DWORD dwErr = GetLastError();
        return false;  
    }  
  
    return true;  
}  
  
  
bool CSendMail::Logon(SOCKET &_socket)  
{  
    recv(_socket,m_cReceiveBuff,1024,0);  
  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    sprintf_s(m_SendBuff,"HELO []\r\n");  
    send(_socket,m_SendBuff,strlen(m_SendBuff),0);//��ʼ�Ự  
    recv(_socket,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='2' || m_cReceiveBuff[1]!='5' || m_cReceiveBuff[2]!='0')  
    {  
        return false;  
    }  
  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    sprintf_s(m_SendBuff,"AUTH LOGIN\r\n");  
    send(_socket,m_SendBuff,strlen(m_SendBuff),0);//�����¼  
    recv(_socket,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='3' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='4')  
    {  
        return false;  
    }  
  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    Char2Base64(m_SendBuff,m_sMailInfo.m_pcUserName,strlen(m_sMailInfo.m_pcUserName));  
    m_SendBuff[strlen(m_SendBuff)]='\r';  
    m_SendBuff[strlen(m_SendBuff)]='\n';  
    send(_socket,m_SendBuff,strlen(m_SendBuff),0);//�����û���  
    recv(_socket,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='3' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='4')  
    {  
        return false;  
    }  
  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    Char2Base64(m_SendBuff,m_sMailInfo.m_pcUserPassWord,strlen(m_sMailInfo.m_pcUserPassWord));  
    m_SendBuff[strlen(m_SendBuff)]='\r';  
    m_SendBuff[strlen(m_SendBuff)]='\n';  
    send(_socket,m_SendBuff,strlen(m_SendBuff),0);//�����û�����  
    recv(_socket,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='2' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='5')  
    {  
        return false;  
    }  
    return true;//��¼�ɹ�  
}  
  
  
bool CSendMail::SendHead(SOCKET &_socket)  
{  
    int rt;  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    sprintf_s(m_SendBuff,"MAIL FROM:<%s>\r\n",m_sMailInfo.m_pcSender);  
    rt=send(_socket,m_SendBuff,strlen(m_SendBuff),0);  
      
    if(rt!=strlen(m_SendBuff))  
    {  
        return false;  
    }  
    recv(_socket,m_cReceiveBuff,1024,0);  
  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    sprintf_s(m_SendBuff,"RCPT TO:<%s>\r\n",m_sMailInfo.m_pcReceiver);  
    rt=send(_socket,m_SendBuff,strlen(m_SendBuff),0);  
    if(rt!=strlen(m_SendBuff))  
    {  
        return false;  
    }  
    recv(_socket,m_cReceiveBuff,1024,0);  

    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    memcpy(m_SendBuff,"DATA\r\n",strlen("DATA\r\n"));  
    rt=send(_socket,m_SendBuff,strlen(m_SendBuff),0);  
    if(rt!=strlen(m_SendBuff))  
    {  
        return false;  
    }  
    recv(_socket,m_cReceiveBuff,1024,0);  
  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    sprintf_s(m_SendBuff,"From:\"%s\"<%s>\r\n",m_sMailInfo.m_pcSenderName,m_sMailInfo.m_pcSender);  
    sprintf_s(&m_SendBuff[strlen(m_SendBuff)],150,"To:\"INVT.COM.CN\"<%s>\r\n",m_sMailInfo.m_pcReceiver);  
    sprintf_s(&m_SendBuff[strlen(m_SendBuff)],150,"Subject:%s\r\nMime-Version: 1.0\r\nContent-Type: multipart/mixed;   boundary=\"INVT\"\r\n\r\n",m_sMailInfo.m_pcTitle);  
    rt=send(_socket,m_SendBuff,strlen(m_SendBuff),0);  
    if(rt!=strlen(m_SendBuff))  
    {  
        return false;  
    }  
  
    return true;  
}  
  
bool CSendMail::SendTextBody(SOCKET &sock)  
{  
    int rt;  
    memset(m_SendBuff,0,sizeof(m_SendBuff));  
    sprintf_s(m_SendBuff,"--INVT\r\nContent-Type: text/plain;\r\n  charset=\"gb2312\"\r\n\r\n%s\r\n\r\n",m_sMailInfo.m_pcBody);  
    rt=send(sock,m_SendBuff,strlen(m_SendBuff),0);  

    if(rt!=strlen(m_SendBuff))  
    {  
        return false;  
    }  
    else  
    {  
        return true;  
    }  
}  
  
bool CSendMail::SendFileBody(SOCKET &sock)  
{  
    int i;  
    char* filePath;  
    int rt;  
    int len;  
    int pt=0;  
    char fileName[128];  
    for(i=0;i<m_FilePathList.GetCount();i++)  
    {  
        pt=0;  
        memset(fileName,0,128);  
        filePath=m_FilePathList.GetAt(m_FilePathList.FindIndex(i));  
        len=GetFileData(filePath);  
        GetFileName(fileName,filePath);  
  
        sprintf_s(m_SendBuff,"--INVT\r\nContent-Type: application/octet-stream;\r\n  name=\"%s\"\r\nContent-Transfer-Encoding: base64\r\nContent-Disposition: attachment;\r\n  filename=\"%s\"\r\n\r\n",fileName,fileName);  
        send(sock,m_SendBuff,strlen(m_SendBuff),0);  
		while (pt<len)  
		{  
			memset(m_SendBuff,0,sizeof(m_SendBuff));  
			Char2Base64(m_SendBuff,&m_pFileBuffer[pt],min(len-pt,3000));  
			m_SendBuff[strlen(m_SendBuff)]='\r';  
			m_SendBuff[strlen(m_SendBuff)]='\n';  
            rt=send(sock,m_SendBuff,strlen(m_SendBuff),0);  
			printf("%s",m_SendBuff);
            pt+=min(len-pt,3000);  
            if(rt!=strlen(m_SendBuff))  
            {  
                return false;  
            }  
        }  
        if(len!=0)  
        {  
            delete [] m_pFileBuffer;  
        }  
    }  
  
    return true;  
}  
  
bool CSendMail::SendEnd(SOCKET &sock)  
{  
    sprintf_s(m_SendBuff,"--INVT--\r\n.\r\n");  
    send(sock,m_SendBuff,strlen(m_SendBuff),0);  
  
    sprintf_s(m_SendBuff,"QUIT\r\n");  
    send(sock,m_SendBuff,strlen(m_SendBuff),0);  
    closesocket(sock);  
    WSACleanup();  
    return true;  
}  
  
  
bool CSendMail::SendMail(sMailInfo &smailInfo)  
{  
    memcpy(&m_sMailInfo,&smailInfo,sizeof(smailInfo));  
    if(m_sMailInfo.m_pcBody==NULL  
        || m_sMailInfo.m_pcIPAddr==NULL  
        || m_sMailInfo.m_pcIPName==NULL  
        || m_sMailInfo.m_pcReceiver==NULL  
        || m_sMailInfo.m_pcSender==NULL  
        || m_sMailInfo.m_pcSenderName==NULL  
        || m_sMailInfo.m_pcTitle==NULL  
        || m_sMailInfo.m_pcUserName==NULL  
        || m_sMailInfo.m_pcUserPassWord==NULL)  
    {  
        return false;  
    }  
    SOCKET sock;  
    if(!CReateSocket(sock))//��������  
    {  
        return false;  
    }  
  
    if(!Logon(sock))//��¼����  
    {  
        return false;  
    }  
  
    if(!SendHead(sock))//�����ʼ�ͷ  
    {  
        return false;  
    }  
  
    if(!SendTextBody(sock))//�����ʼ��ı�����  
    {  
        return false;  
    }  
  
    if(!SendFileBody(sock))//���͸���  
    {  
        return false;  
    }  
  
    if(!SendEnd(sock))//�����ʼ������ر�sock  
    {  
        return false;  
    }  
  
    return true;  
}  
