//����������ϰc++ʵ��
//author:sixbeauty

#include<iostream>
#include<vector>
#include<string>

template<typename T>
void quickSort(std::vector<T> &p,int left,int right)	//��������ʵ�ֺ���
{
	if(left>=right)	return ;	//���ұ�־ΪС�ڻ�������־λ�������κ���

	T keyData=p[left];			//ѡȡ��һλΪkeyֵ
	int	i=left,j=right;			//�����α궨��

	while(i<j)
	{
		while(i<j && p[j]>keyData)	//��һ�ν���ѭ����i=left������p[i]��ֵ����keyData����������Ҫ��֤p[i]=keyData
			j--;				//���α��ƶ�����һ��С��keyֵ��λ��,i<j��֤�����磬��ֹ����ĸ�ֵ����

		p[i]=p[j];				//��ʱp[j]��ֵ����keyData,�����ǲ����ϸ�ֵ

		while(i<j && p[i]<keyData)
			i++;				

		p[j]=p[i];				//������һ�ֺ�p[i]��ֵ�����µ���keyData
	}	//����ѭ������i��ǰ��ֵ��С��keyData,��i�Ժ��ֵ������keyData,��ʱֻҪ��keyData��ֵ��p[i]����
	p[i]=keyData;

	quickSort(p,left,i-1);
	quickSort(p,i+1,right);
}

int main()
{
	//���Դ���
	/*int p1[8]={1,4,5,7,11,2,18,9};
	std::vector<int> vetInt;
	for(int i=0;i<8;i++)
		vetInt.push_back(p1[i]);

	quickSort<int>(vetInt,0,7);

	for(std::vector<int>::iterator itr=vetInt.begin();itr!=vetInt.end();itr++)
		std::cout<<*itr<<",";
	std::cout<<std::endl;*/


	//���Դ���
	std::string p1[8]={"ba","a","abd","defef","c","ca","daaa","aaa"};
	std::vector<std::string> vetStr;
	for(int i=0;i<8;i++)
		vetStr.push_back(p1[i]);

	quickSort<std::string>(vetStr,0,7);

	for(std::vector<std::string>::iterator itr=vetStr.begin();itr!=vetStr.end();itr++)
		std::cout<<*itr<<",";
	std::cout<<std::endl;
}