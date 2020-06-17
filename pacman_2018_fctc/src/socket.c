#include "socket.h"
#include "window.h"
#include <arpa/inet.h>

#define SERV_IP_ADDR "127.0.0.1"
#define PORT_NUM 3088

int port_num = 3088;

void init_server(Socket_value *socket_info,int gameMode){
	/* server socket 초기화 & 연결 client로부터 대기*/
	
	// 소켓 생성
	if((socket_info->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Server : Can't open stream socket\n");
		exit(0);
	}
	
	//server_Addr 을 NULL로 초기화
	memset(&socket_info->server_addr, 0x00, sizeof(socket_info->server_addr));
	printf("port_num: %d\n",port_num);
	//server_addr 셋팅		 
	socket_info->server_addr.sin_family = AF_INET;
	socket_info->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//socket_info->server_addr.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);
	socket_info->server_addr.sin_port = htons(port_num++);
	socket_info->addr_len = 0;

	//2020 ADD test
	socket_info->gameMode = gameMode;

	if(bind(socket_info->server_fd, (struct sockaddr *)&socket_info->server_addr, sizeof(socket_info->server_addr)) <0)
    {
        printf("Server : Can't bind local address.\n");
        //exit(0);
    }
	
	printf("Ip : %s\n", inet_ntoa(socket_info->server_addr.sin_addr));
	printf("Port : %d\n",socket_info->server_addr.sin_port);
	printf("Server : wating connection request.\n");
}

/*
int connect_server(Socket_value *socket_info) {
	//소켓을 수동 대기모드로 설정
	if(listen(socket_info->server_fd, 5) < 0)
	{
		printf("Server : Can't listening connect.\n");
		exit(0);
	}

	//2020 ADD : async socket
	int flag = fcntl(socket_info->server_fd, F_GETFL, 0);
	fcntl(socket_info->server_fd, F_SETFL, flag | O_NONBLOCK);
	int server_msg[1];
	while(1){
		socket_info->client_fd = accept(socket_info->server_fd, (struct sockaddr *)&socket_info->client_addr, &socket_info->addr_len);
		if(socket_info->client_fd > 0){
			recv(socket_info->client_fd,server_msg,sizeof(int),MSG_WAITALL);
			printf("%d\n",server_msg[0]);
			send(socket_info->client_fd,&socket_info->gameMode,sizeof(int),0);
			return 0;
		}		
	}
	//return 0;
}
*/

int connect_server(Socket_value *socket_info) {
	//소켓을 수동 대기모드로 설정
	if(listen(socket_info->server_fd, 5) < 0)
	{
		printf("Server : Can't listening connect.\n");
		exit(0);
	}
	//2020 ADD : async socket
	int server_msg[1];
	int sFlag = fcntl(socket_info->server_fd, F_GETFL, 0);
	fcntl(socket_info->server_fd, F_SETFL, sFlag | O_NONBLOCK);
	
	Uint8 *keystates = SDL_GetKeyState(NULL);
	int writeFlag = 1;

	while(1){
		//accept
		socket_info->client_fd = accept(socket_info->server_fd, (struct sockaddr *)&socket_info->client_addr, &socket_info->addr_len);
		if(socket_info->client_fd < 0){
			//Not yet
			if(errno == EAGAIN){
				if(writeFlag == 1){
					printf("Server : Not accep yet.\n");
					writeFlag = 0;
				}
			}
			//failed
			else{
				printf("Server : accept failed.\n");
				return -1;
			}
		}		
		//accept successly
		else{
			printf("Server : Connect successfuly\n");
			//check ip and start 
			char ip[50] = {'\0',};
			inet_ntop(AF_INET,(struct sockaddr *)&socket_info->client_addr, ip,INET_ADDRSTRLEN);
			
			recv(socket_info->client_fd,server_msg,sizeof(int),MSG_WAITALL);
			printf("client address : %s , %d\n",ip,server_msg[0]);
			send(socket_info->client_fd,&socket_info->gameMode,sizeof(int),0);
			return 0;
		}
		//Other processing
		SDL_PumpEvents(); //update keystates
		
		if(keystates[SDLK_ESCAPE]){ //when escape key is downed.
				printf("Pause the server's waiting\n");
				return 1;
		}
	}
}

int connect_client(Socket_value *socket_info, char *severIP){
	if((socket_info->client_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("can't create socket\n");
		exit(0);
	}
	
	printf("port_num: %d\n",port_num);
	socket_info->server_addr.sin_family = AF_INET;
	socket_info->server_addr.sin_addr.s_addr = inet_addr(severIP);
	socket_info->server_addr.sin_port = htons(port_num++);
    
	//서버로 연결요청
	if(connect(socket_info->client_fd, (struct sockaddr *)&socket_info->server_addr, sizeof(socket_info->server_addr)) < 0)
    {
		printf("Ip : %s\n", inet_ntoa(socket_info->server_addr.sin_addr));
		printf("Port : %d\n",socket_info->server_addr.sin_port);
		printf("can't connect.\n");
		return -1;
	}
	
	printf("Client : wating connection request.\n");
	printf("Ip : %s\n", inet_ntoa(socket_info->server_addr.sin_addr));
	printf("Port : %d\n",socket_info->server_addr.sin_port);
	int client_msg[1] = {9};
	send(socket_info->client_fd,client_msg,sizeof(int),0);
	recv(socket_info->client_fd,client_msg,sizeof(int),MSG_WAITALL);
	printf("%d\n",client_msg[0]);
	socket_info->gameMode = client_msg[0];
	return 1;
}