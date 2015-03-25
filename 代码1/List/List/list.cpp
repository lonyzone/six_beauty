
#include<iostream>
#include<list>
using namespace std;
typedef list<int> INTLIST;
INTLIST::iterator itrsa;
void printlist(list<int>& c0)
{
	for(itrsa=c0.begin();itrsa!=c0.end();itrsa++)
		cout<<*itrsa<<",";
	cout<<endl;
}
int main()
{
	list<int>c0;	//������
	list<int>c1(3);	//3��Ĭ��ֵΪ0������
	list<int>c2(5,2);	//5��Ԫ�أ�ֵΪ2��
	list<int>c4(c2);	//��һ��c2��copy����
	list<int>c5(++c1.begin(),--c1.end());	//c5��c1�Ĳ���Ԫ�س�ʼ��
	c5.assign(c2.begin(),--c2.end());
	

	//empty,clear�ж�����Ϊ��
	cout<<"empty,clear��"<<endl;
	for(itrsa=c5.begin();itrsa!=c5.end();itrsa++)
		cout<<*itrsa<<",";
	if(c5.empty())
		cout<<"empty"<<endl;
	else
		cout<<"no empty"<<endl;
	c5.clear();
	if(c5.empty())
		cout<<"empty"<<endl;
	else
		cout<<"no empty"<<endl;

	//��������Ԫ�� insert�����ص��ǲ���Ԫ�ص�iterator����ǰԪ�ص�ǰһλ
	for(int i=0;i<9;i++)
		c0.push_back(i);
	for(itrsa=c0.begin();itrsa!=c0.end();itrsa++)
		cout<<*itrsa<<",";
	cout<<endl;
	for(itrsa=c0.begin();itrsa!=c0.end();itrsa++)
		if(*itrsa==3)
			c0.insert(itrsa,13);
	for(itrsa=c0.begin();itrsa!=c0.end();itrsa++)
		if(*itrsa==3)
			c0.insert(itrsa,23);
	printlist(c0);

	//�ϲ�����merge
	cout<<"list  merge:"<<endl;
	printlist(c0);
	printlist(c2);
	
	
	int A1[]={1,2,3,4,5,6};
    int A2[]={2,4,6,8,9,10};
    
    // ��������
    int A3[]={1,2,3,4,6,9};
    int A4[]={5,6,7,8,9,2};
    
    //�������� 
    list<int> iL1(A1, A1+6);
    list<int> iL2(A2, A2+6);
    
    //��������
    list<int> iL3(A3, A3+6);
    list<int> iL4(A4, A4+6);
    
    c2.merge(iL1);	//�ϲ�ǰ���붼���Ѿ��ź����list����Ȼ����
	printlist(c2);

	//�����Դ�����sort
	printlist(c0);
	c0.sort();
	printlist(c0);
	
	//list�ı���ɾ��
	int A6[]={1,7,5,4,14,31,37,33,5,7,5,3,4,5,6};
	INTLIST lis6(A6,A6+15);
	for(itrsa=lis6.begin();itrsa!=lis6.end();)
	{
		if(*itrsa==5)
			itrsa=lis6.erase(itrsa);
		else
			itrsa++;
	}
	printlist(lis6);
	return 0;
}

