//���������ʵ�ַ��룬

#include "modelu.h"


template <typename Any>
void Util<Any>::Swap(Any& a, Any& b)
{
	Any sw=a;
	a=b;
	b=sw;
}


template class Util<int>;

