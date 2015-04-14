#pragma once
#include "curl/curl.h"
#include "httpSessionMaster.h"
#include <string>

class CurlAutoRelease
{
public:
	CurlAutoRelease(){	p = curl_easy_init();}
	~CurlAutoRelease(){	curl_easy_cleanup(p);}
	CURL *GetPtr(){	return p;}
protected:
	CURL* p;
};

class HttpRqWorker: public HttpSessionWorkBase
{
	HttpRqWorker(){};
	~HttpRqWorker(){};
protected:
public:
	virtual void HandleTask(void* pBuffer,WORD wSize);

private:
	CThreadLock m_threadLock;
};

//////////////////////////////////////////////////////////////////

class HttpRqHandler
{
public:
	static HttpRqHandler& getInstance()
	{
		static HttpRqHandler singleInstance;
		return singleInstance;
	}

//	void Init();
//	void StopService();

	bool PostTask(std::string szUrl,std::string szContext,WORD contextSize); 
	HttpRqHandler();
	~HttpRqHandler();

private:
	//������ʵ�ֵĸ��ƹ��캯���͡�=��������
	HttpRqHandler(HttpRqHandler&);
	HttpRqHandler& operator =(const HttpRqHandler&);

private:
	HttpSessionMaster<HttpRqWorker>		m_SessionMaster;
};