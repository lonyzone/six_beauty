#include<iostream>
#include<vector>
using namespace std;
typedef vector<int> INTVECTOR;

int main()
{	
	//vec1�����ʼΪ��
	INTVECTOR vec1;
	//vec2���������10��ֵΪ6��Ԫ��
	INTVECTOR vec2(10,6);
	//vec3���������3��ֵΪ6��Ԫ�أ���������
	INTVECTOR vec3(vec2.begin(),vec2.begin()+3);
	//����һ����Ϊitrsa��˫�������
/*	INTVECTOR::iterator itrsa;
	
	//��ǰ�����ʾvec1�е�����
	cout<<"vec1.begin()--vec1.end():"<<endl;
	for (itrsa =vec1.begin(); itrsa !=vec1.end(); itrsa++)
	{
		cout<<"falg"<<endl;
		cout << *itrsa << " ";
	}
	cout<<endl;
	//��ǰ�����ʾvec2�е�����
	cout<<"vec2.begin()--vex2.end():"<<endl;
	for(itrsa=vec2.begin();itrsa!=vec2.end();itrsa++)
	  cout<< *itrsa<< " ";
	cout<<endl;
	//��ǰ�����ʾvec3�е�����
	cout<<"vec3.begin()--vec3.end():"<<endl;
	for(itrsa=vec3.begin();itrsa!=vec3.end();itrsa++)
		cout<<*itrsa<<" ";
	cout<<endl;
	
	//������ӺͲ����Ա������vector��֧�ִ�ǰ����
	vec1.push_back(2);	//�Ӻ������һ����Ա
	vec1.push_back(4);
	//��vec1��һ��λ�ÿ�ʼ����vec3�����г�Ա
	vec1.insert(vec1.begin(),vec3.begin(),vec3.end());
	cout<<"aftre push() and insert() now the vec1 is:"<<endl;
	for(itrsa=vec1.begin();itrsa!=vec1.end();itrsa++)
	  cout<< *itrsa<<" ";
	cout<<endl;

	//���Ը�ֵ��Ա����
	vec2.assign(8,1);	//���¸�vec2���ƣ�8����Ա�ĳ�ʼֵ��Ϊ1
	cout<<"vec2.assign(8,1):"<<endl;
	for(itrsa=vec2.begin();itrsa!=vec2.end();itrsa++)
	  cout<< *itrsa<< " ";
	cout<<endl;

	vec2.assign(vec1.begin(),vec1.end());
	cout<<"vec2.assign(vec1.begin(),vec1.end()):"<<endl;
	for(itrsa=vec2.begin();itrsa!=vec2.end();itrsa++)
	  cout<< *itrsa<< " ";
	cout<<endl;

	//���������ຯ��
	cout<<"vec1.front()="<<vec1.front()<<endl;	//vec1�������Ա
	cout<<"vec1.back()="<<vec1.back()<<endl;	//vec1�����һ����Ա
	cout<<"vec1.at(2)="<<vec1.at(2)<<endl;		//vec1�ĵ�5����Ա
	cout<<"vec1[4]="<<vec1[4]<<endl;
	*/
	INTVECTOR::iterator itrsa;
	//�����Ƴ���ɾ��
	for(int i=1;i<8;i++)
		vec1.push_back(i);
	vec1.push_back(7);
	vec1.push_back(7);
	vec1.push_back(7);
	//��ǰ�����ʾvec1�е�����
	cout<<"vec1.begin()--vec1.end():"<<endl;
	for (itrsa =vec1.begin(); itrsa !=vec1.end(); itrsa++)
	{
		cout << *itrsa << " ";
	}
	cout<<endl<<"flag"<<endl;
/*	for(itrsa=vec1.begin();itrsa!=vec1.end();)
	{
		if(*itrsa==7)
			itrsa=vec1.erase(itrsa);
		else
			itrsa++;		
	}
	*/
	//��ǰ�����ʾvec1�е�����
	cout<<"vec1.begin()--vec1.end():"<<endl;
	for (itrsa =vec1.begin(); itrsa !=vec1.end(); itrsa++)
	{
		cout << *itrsa << " ";
	}
	
	return 0;
}