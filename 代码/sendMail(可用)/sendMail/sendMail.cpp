// sendMail.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "sendMail.h"
#include "CSendMail.h"

using namespace std;

void sa_sendmail()
{
	//sMailInfo sm;
	//sm.m_pcUserName = "xujingping@lkgame.com";//"";
	//sm.m_pcUserPassWord ="xjp000";// "";
	//sm.m_pcSenderName = "������";
	//sm.m_pcSender = "xujingping@lkgame.com";//�����ߵ������ַ  
	//sm.m_pcReceiver = "sanyue9394@163.com";//�����ߵ������ַ  
	//sm.m_pcTitle = "���������ʼ�";//�������  
	//sm.m_pcBody = "������ٷ����¸�������һҹ�����𣿷��� http://www.sb.com";//�ʼ��ı�����  
	//sm.m_pcIPAddr = "" ;//��������IP�������� 
	//sm.m_pcIPName ="smtp.lkgame.com" ;//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�  
	sMailInfo sm;
	lstrcpyn(sm.m_pcUserName , "sanyue9394@126.com",128);//"",128);
	lstrcpyn(sm.m_pcUserPassWord ,"sanyue214008",128);// "",128);
	lstrcpyn(sm.m_pcSenderName , "six_beauty�Ƽ�",128);
	lstrcpyn(sm.m_pcSender , "sanyue9394@126.com",128);//�����ߵ������ַ  
	lstrcpyn(sm.m_pcReceiver , "sanyue9394@163.com",128);//�����ߵ������ַ  
	lstrcpyn(sm.m_pcTitle , "From:sendMail,���ƻ������ˣ�����",128);//�������  
	lstrcpyn(sm.m_pcBody , "������ٷ����¸�������һҹ�����𣿷��� http://www.sb.com",1024);//�ʼ��ı�����  
	lstrcpyn(sm.m_pcIPAddr , "" ,128);//��������IP�������� 
	lstrcpyn(sm.m_pcIPName ,"smtp.126.com" ,128);//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�

	CSendMail csm;
	//csm.AddFilePath("F:\\mailfile\\55e736d12f2eb938e9ea5eb1d5628535e4dd6fe9.jpg");
	csm.AddFilePath("F:\\mailfile\\libcurl.exp");
	//while(TRUE)
	{
		csm.SendMail(sm);
		//Sleep(1000 * 20);
	}
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	freopen("F://mailfile//sendmail_out.txt","w+",stdout);
	sa_sendmail();
	return 0;
}
