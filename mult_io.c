#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<fcntl.h>
#include<time.h>
#include<string.h>
#include<errno.h>
#include<sys/select.h>
#define _SERVER_IP "127.0.0.1"
#define _SERVER_PORT 9999
#define _BACKLOG_ 128
#define MSG "Please Try Again"
int net_initializer(char* ip,int port){
	
	int server_sock;
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_sock=socket(AF_INET,SOCK_STREAM,0);

	if(!ip){
		server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		}
	else{
		server_addr.sin_addr.s_addr=inet_addr(ip);
	}

	if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
		perror("net initiliazer fail");
		}
	listen(server_sock,_BACKLOG_);
	return server_sock;
	}

int getmaxfd(fd_set* setptr){
	int maxfd = -1;
	    for(int fd = 0; fd < FD_SETSIZE; ++fd){
			if (FD_ISSET(fd, setptr)) {
				if (fd > maxfd)
			       maxfd = fd;
				}
		    }
		return maxfd;
	}

int first_response(int sock,char* cip){
	
	//首次响应
	char response[1000];
	bzero(response,sizeof(response));
	sprintf(response,"Hi,[%s] connect server success\n",cip);
	send(sock,response,strlen(response),0);
	return 0;
	
	}
int server_start(void){
	int client_sock;
	int server_sock;
	int client_io_array[1000];
	struct sockaddr_in client_addr;
	socklen_t addrlen;
	char response[1500];
	int recvlen;
	char cip[16];
	time_t tp;
	char time_buf[1024];
	char* msg="Please try again\n";
	for(int i=0;i<1000;i++)
		client_io_array[i] = -1;
	server_sock=net_initializer(_SERVER_IP,_SERVER_PORT);
	//设置select
	fd_set set,oset;
	FD_ZERO(&set);
	FD_ZERO(&oset);
	FD_SET(server_sock,&set);
	//-----------------------

	//设置非阻塞的sock
	int flag;
	flag=fcntl(server_sock,F_GETFL);//获取属性
	flag |=O_NONBLOCK;
	fcntl(server_sock,F_SETFL,flag);

	printf("Server start accepting\n");//服务器正在接受

	while(1){
		addrlen=sizeof(client_addr);
		oset=set;
		//用select代替原本单纯的accept等待
		int readynum=select(getmaxfd(&oset)+1,&oset,NULL,NULL,NULL);
		
		//如果返回值大于0,就绪socket>0,进行连接，将这个socket存进数组里
		if(readynum>0){
			if(FD_ISSET(server_sock,&oset))
				if((client_sock=accept(server_sock,(struct sockaddr*)&client_addr,&addrlen))>0){
					for(int i=0;i<1000;i++){
	            	    if(client_io_array[i]==-1){
			        	      client_io_array[i]=client_sock;
				        	    break;
				            	}
			       			 }
				//添加进入监听集合
				FD_SET(client_sock,&set);
				bzero(cip,16);
				inet_ntop(AF_INET,&client_addr.sin_addr,cip,16);
				printf("connect success cip %s cport %d\n",cip,ntohs(client_addr.sin_port));
				first_response(client_sock, cip);
				}
		//连接失败
			if(client_sock==-1){
			    if(errno==EAGAIN){}
				else{
					perror("server accept error\n");
					exit(0);
					}
				}

			}
		if(readynum==0)continue;		
			
	//client sock IO的遍历处理

	for(int i=0;i<1000;i++){
		//如果存了sock（不是-1）
		if(client_io_array[i]==-1)continue;
		if (!FD_ISSET(client_io_array[i], &oset)) continue;
		
			bzero(response,sizeof(response));
			if((recvlen=recv(client_io_array[i],response,sizeof(response),MSG_DONTWAIT))>0){
					
					response[(strlen(response))-1]='\0';
					if(strcmp(response,"time")==0){
						tp=time(NULL);
						bzero(time_buf,sizeof(time_buf));
						ctime_r(&tp,time_buf);
						send(client_io_array[i],time_buf,strlen(time_buf),MSG_NOSIGNAL);
						}
			
					else{
						
						send(client_io_array[i],msg,strlen(msg),MSG_NOSIGNAL);
						}

				}
			else if(recvlen==0){
					printf("client exit, close client sock = %d\n",client_io_array[i]);
					close(client_io_array[i]);
					FD_CLR(client_io_array[i],&set);
					client_io_array[i]=-1;
					
				}

			else{
				
					if(errno==EAGAIN){}
					else{
						perror("server_start:recv call failed!");
						exit(0);
						}
				}
		}
	}
}
int main(){
	
	server_start();
	}
