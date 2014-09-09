#include<iostream>
#include<cstring>
using namespace std;

int hight,wide;
int mazeArr[41][41];

struct stepNode
{
	int h;
	int w;
};
int dirt;
stepNode step;
stepNode startStep;
stepNode endStep;

inline bool l_step()			//��f,��b,��l,��r
{
	if(mazeArr[step.h][step.w-1]==0 || mazeArr[step.h][step.w-1]==3 )	
	{
		step.w=step.w-1;
		dirt=4;
		return true;
	}
	return false;
}
inline bool f_step()
{
	if(mazeArr[step.h+1][step.w]==0 || mazeArr[step.h+1][step.w]==3)	
	{
		step.h=step.h+1;
		dirt=1;
		return true;
	}
	return false;
}
inline bool r_step()
{
	if(mazeArr[step.h][step.w+1]==0 || mazeArr[step.h][step.w+1]==3)	
	{
		step.w=step.w+1;
		dirt=3;
		return true;
	}
	return false;
}
inline bool b_step()
{
	if(mazeArr[step.h-1][step.w]==0 || mazeArr[step.h-1][step.w]==3)	
	{
		step.h=step.h-1;
		dirt=2;
		return true;
	}
	return false;
}

void getLNextStep()
{
	switch(dirt)
	{
	case 1:
		{
			(l_step()||f_step()||r_step()||b_step());
			break;
		}
	case 2:
		{
			(r_step()||b_step()||l_step()||f_step());
			break;
		}
	case 3:
		{
			(f_step()||r_step()||b_step()||l_step());
			break;
		}
	case 4:
		{
			(b_step()||l_step()||f_step()||r_step());
			break;
		}
	}
}

void getRNextStep()
{
	switch(dirt)
	{
	case 1:
		{
			(r_step()||f_step()||l_step()||b_step());
			break;
		}
	case 2:
		{
			(l_step()||b_step()||r_step()||f_step());
			break;
		}
	case 3:
		{
			(b_step()||r_step()||f_step()||l_step());
			break;
		}
	case 4:
		{
			(f_step()||l_step()||b_step()||r_step());
			break;
		}
	}
}
int doSearch(int ltRt)	//ltRt: 0��ʾleft,1��ʾright
{
	int count=1;
	step.h=startStep.h;
	step.w=startStep.w;

	//ȷ������
	if(startStep.h==0)
		dirt=1;			//h=1,w=0;
	else if(startStep.h==hight)
		dirt=2;			//h=-1,w=0;
	else if(startStep.w==0)
		dirt=3;			//h=0,w=1;
	else if(startStep.w==wide)
		dirt=4;			//h=0,w=-1;

	while(1)
	{
		if(mazeArr[step.h][step.w]==3)
		{
			return count;
		}
		//δ����
		if(ltRt)	//1��ʾright
		{
			getRNextStep();
			count++;
		}else
		{
			getLNextStep();
			count++;
		}
	}//while
}

struct DSFNode
{
	int x;
	int y;
	int step;
};
int main()
{
	freopen("E:/����2/poj_3083/Debug/in.txt","r",stdin);
	int n;
	cin>>n;
	for(int s=0;s<n;s++)
	{
		cin>>wide>>hight;
		char c;
		for(int s=hight-1;s>=0;s--)
		{
			for(int y=0;y<wide;y++)
			{
				cin>>c;
				if(c=='#')	mazeArr[s][y]=1;
				else if(c=='.')	mazeArr[s][y]=0;
				else if(c=='S')	
				{
					mazeArr[s][y]=2;
					startStep.h=s;
					startStep.w=y;
				}
				else if(c=='E')	
				{
					mazeArr[s][y]=3;
					endStep.h=s;
					endStep.w=y;
				}
			}
		}//ѭ������

		int leftstep=doSearch(0);
		int rightstep=doSearch(1);

		cout<<leftstep<<" "<<rightstep<<" "<<endl;
	}
	
	return 0;
}

