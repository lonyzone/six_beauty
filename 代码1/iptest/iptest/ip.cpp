#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*��ʮ�����Ʋ���ת���ɵ��ʮ���� 
���磺0x8002c2f2  ->128.2.194.242
*/
int my_htonl(char *argv)
{
	struct in_addr inaddr;//�����ֽ���
	unsigned int addr;//���ʮ����

	sscanf(argv, "%x", &addr);
	inaddr.s_addr = htonl(addr);
	printf("%s/n", inet_ntoa(inaddr));

	return 0;
}


/*�����ʮ���Ʋ���ת����ʮ������
���磺 128.2.194.242->0x8002c2f2
*/
int my_ntohl(char * argv)
{
	struct in_addr inaddr;//�����ֽ���
	unsigned int addr;//���ʮ����

	if(inet_aton(argv, &inaddr) != 0){
		addr = ntohl(inaddr.s_addr);
		printf("0x%x/n", addr);
	}
	return 0;
}



int main( )
{
	char * test_arry1  = "0xcb00a8c0";

          
	return 0;
}