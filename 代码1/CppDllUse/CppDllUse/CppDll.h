//CppDll.h

//ʹ��Ԥ���뿪���л���ĵ��뵼������
#ifdef DLLCLASS_EXPORTS

#define EXT_CLASS _declspec(dllexport)//DLL

#else

#define EXT_CLASS _declspec(dllimport)//ʹ����

//��������
EXT_CLASS int CPPdll_sub(int sub1,int sub2);
EXT_CLASS int CPPdll_add(int add1,int add2);
EXT_CLASS int CPPdll_mul(int mul1,int mul2);


#endif