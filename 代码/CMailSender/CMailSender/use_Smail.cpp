#include "CSendMail.h"
#include "iostream"
using namespace std;


int main()
{
	CSendMail sMailer;
//	freopen("F://mailfile//out.txt","w+",stdout);
	
	sMailer.setServerName("smtp.126.com");														//����smtp,��"smtp.126.com"
	sMailer.setUserName(userName);																//�����˺�������"****@126.com"
	sMailer.setUserPwd(userPwd);																//��������
	sMailer.setSenderName("�¼���");																//����������
	sMailer.setSenderAddress("sanyue9394@126.com");												//���������ַ�������˺ŵĵ�ַ,����������˺���"****@126.com"

	sMailer.setReceiver("show_ball","939445950@qq.com");										//����ʼ�������
	sMailer.addReceiver("six_beauty","sanyue9394@126.com");

	sMailer.AddFilePath("F:\\mailfile\\55e736d12f2eb938e9ea5eb1d5628535e4dd6fe9.jpg");			//��Ӹ���
	sMailer.AddFilePath("F:/mailfile/libcurl.exp");												//��Ӹ���

	if(sMailer.Connent())																		//ÿ�η��ʼ�ǰ����Ҫconnect
	{
		if(sMailer.SendMail("CMailSender:����������","������ٷ����¸�������һҹ�����𣿷��� http://www.sb.com"))			//��һ���ַ������ʼ����⣬�ڶ������ʼ�����
				cout<<"�ʼ�������ɣ�";

	}	

	if(sMailer.Connent())																		//ÿ�η��ʼ�ǰ����Ҫconnect
	{
		std::string title,content;
		title="�����ʼ�part_2";
		content="�����Ǹ���˸£�";
		if(sMailer.SendMail(title,content))														//��һ���ַ������ʼ����⣬�ڶ������ʼ�����
				cout<<"�ʼ�������ɣ�";
	
	}
	

	return 0;
}