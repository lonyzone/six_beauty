// IOCPserver.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TcpSrv.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TcpSrv myserver;
	myserver.startup();
	myserver.closeIocp();
	return 0;
}

