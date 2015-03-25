#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 100001;

int N, K;

int vis[MAXN];
int ret[MAXN];

queue<int> q;

int BFS(int s, int d)
{
	if (s == d) return 0;
	q.push(s);
	
	int cur;
	
	while (!q.empty())
	{
		cur = q.front();
		q.pop();
		
		//if (cur == d) break;
		
		// vis[cur] = 1;
		
		/*
			ע��BFS�ĸ������������ǣ������㲻���յ㣬����������С�Ȼ�����������Χ�Ľڵ㡣ע����Ϊ���ʵ�ʱ�����Ҫ��
			��������POP��ʱ���ǣ���������POP���ڵ�Ϊ����������ɨ���ʱ�򣬽���Χ���ھӱ�ǡ����ҿ����ھ��Ƿ�Ϊ�յ㡣�����ǿ���POP
			�����Ǹ��ڵ��Ƿ�Ϊ�յ㡣���������ܱ�֤BFS�ҵ��ĵ�һ���������Ž⡣
		*/
		
		if (cur + 1 < MAXN && !vis[cur + 1])
		{
			q.push(cur + 1);
			ret[cur + 1] = ret[cur] + 1;
			vis[cur + 1] = 1;
		}
		
		if (cur + 1 == d) break;
		
		if (cur - 1 >= 0 && !vis[cur - 1])
		{
			q.push(cur - 1);
			ret[cur - 1] = ret[cur] + 1;
			vis[cur - 1] = 1;
		}
		
		if (cur - 1 == d) break;
		
		if (cur << 1 < MAXN && !vis[cur << 1])
		{
			q.push(cur << 1);
			ret[cur << 1] = ret[cur] + 1;
			vis[cur << 1] = 1;
		}
		
		if (cur << 1 == d) break;
	};
	
	return ret[d];
}

int main()
{
	scanf("%d %d", &N, &K);
	printf("%d/n", BFS(N, K));
	return 0;
}