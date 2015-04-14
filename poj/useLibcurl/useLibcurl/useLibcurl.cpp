#include<iostream>
#include<string>
#include"curl/curl.h"
using namespace std;

class CurlAutoRelease
{
public:
	CurlAutoRelease(){	p = curl_easy_init();}
	~CurlAutoRelease(){	curl_easy_cleanup(p);}
	CURL *GetPtr(){	return p;}
protected:
	CURL* p;
};

UINT CurlWriteBuffer(char *buffer,UINT size,UINT nmemb,std::string* stream)
{
	UINT sizes = size*nmemb;
	if(stream == NULL) return 0;
	stream->append(buffer,sizes);
	return sizes;
}

int main()
{
	curl_global_init(CURL_GLOBAL_DEFAULT); 
	
	std::string szUrl("www.baidu.com");
	std::string szContent("");

	try
	{
		CurlAutoRelease curlAutoRelease;
		CURL* curl = curlAutoRelease.GetPtr();
		CURLcode res;

		std::string strResult;
		{
			curl_easy_setopt(curl,CURLOPT_URL,szUrl.c_str());
			curl_easy_setopt(curl,CURLOPT_VERBOSE,1);								//�򿪵���
			curl_easy_setopt(curl,CURLOPT_POSTFIELDS,szContent.c_str());			//post����	
			curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,szContent.length());
			curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,CurlWriteBuffer);			//д�ص�
			curl_easy_setopt(curl,CURLOPT_WRITEDATA,&strResult);					//д�洢
			curl_easy_setopt(curl,CURLOPT_TIMEOUT,10);								//�ִ��ʱ��

			res = curl_easy_perform(curl);
		}

		if(res != CURLE_OK)
		{
			//Ͷ��Http requeset ʧ��
			std::cout<<"Ͷ��Http����ʧ�ܣ�"<<std::endl;
			return -1;
		}
		else
		{
			std::cout<<strResult<<std::endl;
		}
	}
	catch (...)
	{
		//��Exception
		std::cout<<"HttpRqWorker::HandleTask������Exception!!"<<std::endl;
	}


	curl_global_cleanup();
	return 0;
}
