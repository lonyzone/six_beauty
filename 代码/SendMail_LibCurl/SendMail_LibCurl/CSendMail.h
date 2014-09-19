#ifndef CSENDMAIL_H
#define	CSENDMAIL_H

#include <string>
#include <list>
#include <vector>
#include "curl/curl.h"

#define MULTI_PERFORM_HANG_TIMEOUT 60 * 1000


class CSendMail {
public:
    CSendMail();
    CSendMail(  //create sendmail object with paremeter;
                const std::string & strUser,
                const std::string & strPsw, 
                const std::string & strSmtpServer, 
                int iPort, 
                const std::string & strMailFrom
            );
    CSendMail(const CSendMail& orig);
    virtual ~CSendMail();
private:
    std::string m_strUser;			//�����û���
    std::string m_strPsw;			//��������
    std::string m_strSmtpServer;		//����SMTP������
    int         m_iPort;			//����SMTP�������˿�
    std::list<std::string> m_RecipientList;  	//�������ʼ�list
    std::string m_strMailFrom;			//����������
    std::vector<std::string> m_MailContent;	//���͵����ݶ��У�����ͷ��������
    int         m_iMailContentPos;		//���ڷ�������ʱ��¼���͵��ڼ���content
    
private:
    //�������ݻص�����
    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp);
    //��ȡ��ǰʱ��
    static struct timeval tvnow(void);
    //����ʱ���
    static long tvdiff(struct timeval newer, struct timeval older);
    //�����ʼ�����
    bool ConstructHead(const std::string & strSubject/*�ʼ�����*/, const std::string & strContent/*�ʼ�����*/);
    
public:
    
    bool SendMail(const std::string & strSubject, const char * pMailBody, int len);
    bool SendMail(const std::string & strSubject, const std::string & strMailBody);
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    std::list<std::string> & recipientList,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const char * pMailBody, 
                    int len
                );
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    const std::string & strMailTo,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const char * pMailBody, 
                    int len
                );
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    std::list<std::string> & recipientList,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const std::string & strMailBody
                );
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    const std::string & strMailTo,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const std::string & strMailBody
                );
    
    void SetUser(const std::string & strUser) { m_strUser = strUser; }
    std::string & GetUser() { return m_strUser; }
    
    void SetPsw(const std::string & strPsw) { m_strPsw = strPsw; }
    std::string & GetPsw() { return m_strPsw; }
    
    void SetSmtpServer(const std::string & strSmtpServer) { m_strSmtpServer = strSmtpServer; }
    std::string & GetSmtpServer() { return m_strSmtpServer; }
    
    void SetPort(int iPort) { m_iPort = iPort; }
    int GetPort() { return m_iPort; }
    
    void SetMailFrom(const std::string & strMailFrom) { m_strMailFrom = strMailFrom; }
    std::string & GetMailFrom() { return m_strMailFrom; }
    
    //��ӽ���������
    void AddRecipient(const std::string & strMailTo) { m_RecipientList.push_back(strMailTo); }
    void AddRecipient(std::list<std::string> recipientList) 
    { 
        std::copy(recipientList.begin(), recipientList.end(), m_RecipientList.begin());
    }
    void ClearRecipient() { m_RecipientList.clear(); }
    
};


#endif	/* CSENDMAIL_H */
