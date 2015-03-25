//2014-4-2  programed by sixbeauty
//zeromp ͨѶ����server ��
#include "zmq.h"
#include "zmq_utils.h"            //Zeromq �����ĵ��������������ʵ����
#include <iostream>
#pragma comment(lib,"libzmq-v100-mt-4_0_4.lib")
int main()
{
    std::cout << "-------------------Zeromq REP style-----------------------" <<std::endl;
    std::cout << "---------------------Author:SanYue------------------------" <<std::endl;

    void* context=zmq_init(1);    //ָ��zmq ����I/0�¼���thread pool Ϊ1
    void* z_socket=zmq_socket(context,ZMQ_PULL);

    zmq_bind(z_socket,"tcp://*:5555");    // accept connections on a socket

	int recvn=1;
    while(1)    //ѭ������
    {    
        //���ܲ���
        zmq_msg_t recv_msg;
        zmq_msg_init(&recv_msg);
        zmq_msg_recv(&recv_msg,z_socket,0);                            //0��ʾ������
        std::cout<<"��\t"<<recvn++<<"\t���յ�client��Ϣ��\t";
        std::cout<<(char*)zmq_msg_data(&recv_msg)<<std::endl;
        zmq_msg_close(&recv_msg);
		/*
		//���Ͳ���
		zmq_msg_t send_msg;
		zmq_msg_init_size(&send_msg,6);
		memcpy(zmq_msg_data(&send_msg),"world",6);
		zmq_sendmsg(z_socket,&send_msg,0);
		zmq_msg_close(&send_msg);
        */
    }

    zmq_close(z_socket);
    zmq_term(context);
            
    return 0;
}