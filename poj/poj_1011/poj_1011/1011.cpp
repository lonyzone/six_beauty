#include<iostream>
#include<algorithm>
using namespace std;

int stick[65];
bool vist[65];
int size;	//stick�����С
int maxLen;	//stick�ܳ���
int nroot;	//����n��

bool cmp(const int a,const int b)
{
	return a>b;
}

bool dfs(const int lstick,	//Ҫƴ�ɵĳ���
		 int len,			//��ǰ����
		 int pos,			//stick����鵽��λ��
		 int root)			//��ǰ�ڼ���
{
	if(len==lstick)	return true;

	return false;
}

int main()
{
	freopen("E:/����2/extra_test/Debug/in.txt","r",stdin);
	while(cin>>size && size)
	{
		maxLen=0;
		for(int s=0;s<size;s++)
		{
			cin>>stick[s];
			maxLen+=stick[s];
		}

		sort(stick,stick+size,cmp);
		for(int s=stick[0];s<=maxLen;s++)
		{
			if(maxLen%s!=0)
				continue;
			//doSearch
			memset(vist,0,sizeof(vist));
			nroot=maxLen/s;
			if(dfs(s,0,0,0))
			{
				cout<<s<<endl;
				break;
			}
		}

	}
	return 0;
}
