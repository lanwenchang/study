/*********************************************
* 单机端口开放范围：
*	look in /proc/sys/net/ip_local_port_range
* 	change in /etc/sysctl.conf(sysctl -p)
* 单进程最大文件限制
	ulimit -n 10000
*********************************************/
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>


int setnonblock(int fd)
{
	int flag = fcntl(fd,F_GETFL,0);
	if ( flag == -1 )
	{
		perror("setnonblock F_GETFL fail:");
		return -1;
	}
	flag |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL,flag);
	if ( flag == -1 )
	{
		perror("setnonblock F_SETFL fail:");
		return -1;
	}
	return 0;
}

int mysock()
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if ( sock == -1 )
	{
		perror("mysock fail:");
	}
	printf("sock %d\n",sock);
	return sock;
}

int myconnect(int fd, char* ip, unsigned short port )
{
	int ret;
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	socklen_t len = sizeof(struct sockaddr);
	ret = connect(fd,(struct sockaddr*)&addr,len);
	if ( ret == -1 )
	{
		perror("myconnect connect fail:");
		return -1;
	}
	if ( setnonblock(fd) == -1  )
	{
		printf("myconnect setnonblock fail\n");
		return -1;
	}
	return 0;
}

int myread(int fd, char* buf, int len )
{
	int bytes_write;
	int bytes_read = read(fd,buf,len);
	buf[bytes_read] = 0;
	return bytes_read; 
}
int mywrite(int fd, char* buf, int len )
{
	int write_len = len;
	int bytes_write;
	while ( len > 0 )
	{
		bytes_write = write(fd,buf,len);
		if ( bytes_write < 0  )
		{
			if ( errno == EINTR )
				bytes_write = 0;
			else
				return -1;
		}
		if ( bytes_write == 0 )
			break;
		buf += bytes_write;
		len -= bytes_write;
	}
	return write_len - len;
}
void* tread(void* arg)
{
	int ret =-1;
	fd_set fset;
	FD_ZERO(&fset);

	int sock = *((int*)arg);

	FD_SET(sock,&fset);

	struct timeval tm;
	tm.tv_sec = 3;
	tm.tv_usec = 0;
	while(1)
	{
		ret = select(sock+1,&fset,NULL,NULL,&tm);
		if ( ret == -1 )
		{
			printf("select fail\n");
		}
		else if ( ret == 0)
		{
			printf("time out\n");	
		}

		if ( FD_ISSET(sock,&fset) )
		{
			char buf[1024];
			int len = sizeof(len);
			if ( len ==  myread(sock,buf,len) )
			{
				printf("recv %s\n",buf);
			}
		}
		sleep(1);
	}

}
void* twrite(void* arg)
{
	int sock = *(int*)arg;
	int ret =-1;
	fd_set fset;
	FD_ZERO(&fset);
	FD_SET(sock,&fset);

	struct timeval tm;
	tm.tv_sec = 3;
	tm.tv_usec = 0;
	while(1)
	{
		ret = select(sock+1,NULL,&fset,NULL,&tm);
		if ( ret == -1 )
		{
			printf("select fail\n");
		}
		else if ( ret == 0)
		{
			printf("time out\n");	
		}

		if ( FD_ISSET(sock,&fset) )
		{
			char* buf = "hello";
			int len = strlen(buf);
			if ( len ==  mywrite(sock,buf,len) )
			{
				printf("mywrite len=%d\n",len);
			}
		}
	}
}
int doconn()
{
	for ( int i=0; i<1000; i++ )
	{
		int ret;
		int sock = mysock();
		if (sock == -1 )
			return -1;
		ret = myconnect(sock,(char*)"127.0.0.1",6666);
		//if ( ret == -1 )
		//	return -1;
		//twrite((void*)sock);
	}
	printf("pid %d sleep\n",getpid());
	sleep(60000);	
}

int main()
{
	int pcount=63;
	int pid=-1;
	for ( int j=0; j<pcount; j++ )
	{
		pid = fork();
		if ( pid == 0 )
		{
			doconn();
			exit(0);
		}
	}

	printf("main sleep\n");
	sleep(10000);

	/*
	pthread_t tid1;
	ret = pthread_create(&tid1,NULL,tread,(void*)&sock);
	if ( ret == -1 )
	{
		perror("create thread1 fail:");
		return -1;
	}
	pthread_t tid2;
	ret = pthread_create(&tid2,NULL,twrite,(void*)&sock);
	if ( ret == -1 )
	{
		perror("create thread2 fail:");
		return -1;
	}

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	*/

	return 0;

}
