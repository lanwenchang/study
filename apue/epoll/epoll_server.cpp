/*****************************************************
*
*
*****************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define MAX_EVENTS  10
#define MAX_LINE_SIZE 1024


int setnonblocking(int fd)
{
	int flags = fcntl(fd,F_GETFL,0);
	if ( flags == -1 )
	{
		perror("setnonblocking F_GETFL fail:");
		return -1;
	}
	flags |= O_NONBLOCK;
	int ret = fcntl(fd,F_SETFL,flags);
	if ( ret == -1 )
	{
		perror("setnonblocking F_SETFL fail:");
		return -1;
	}
	return 0;
}

int main()
{
	struct epoll_event ev,events[MAX_EVENTS];
	int listen_sock, conn_sock, nfds, epollfd,sock_fd;
	struct sockaddr_in addr,local;
	socklen_t addrlen = sizeof(struct sockaddr);
	char line[MAX_LINE_SIZE];
	int len;
	int ret;

	//socket
	listen_sock = socket(AF_INET,SOCK_STREAM,0);
	if ( listen_sock == -1 )
	{
		perror("socket:");
		return -1;
	}
	if ( setnonblocking(listen_sock) == -1 )
	{
		return -1;
	}

	//bind
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family=AF_INET;
	addr.sin_port = htons(6666);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = bind(listen_sock,(struct sockaddr*)&addr, sizeof(struct sockaddr));
	if ( ret == -1 )
	{
		perror("bind:");
		return -1;
	}
	//listen
	ret = listen(listen_sock,10000);
	if ( ret == -1 ) {
		perror("listen:");
		return -1;
	}

	//epoll_create
	epollfd = epoll_create(10);
	if ( epollfd == -1 )
	{
		perror("epoll_create:");
		exit(EXIT_FAILURE);
	}

	//epoll_ctl
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if ( epoll_ctl(epollfd,EPOLL_CTL_ADD,listen_sock,&ev) == -1 )
	{
		perror("expoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}

	int conn_count=0;
	//epoll_wait
	for ( ;; )
	{
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1 );
		if ( nfds == -1 )
		{
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		printf("nfds=%d\n",nfds);
		for (int n=0; n<nfds; ++n )
		{
			if ( events[n].data.fd == listen_sock )
			{
				conn_sock = accept(listen_sock,(struct sockaddr*)&addr,&addrlen);
				printf("a connection\n");
				if ( conn_sock == -1 )
				{
					perror("accept");
					exit(EXIT_FAILURE);
				}

				setnonblocking(conn_sock);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_sock;
				if ( epoll_ctl(epollfd,EPOLL_CTL_ADD,conn_sock,&ev) == -1 )
				{
					perror("epoll_ctl");
					exit(EXIT_FAILURE);
				}
				conn_count++;
				printf("-------------------------------------connnect count=%d\n",conn_count);
			}
			else if ( events[n].events & EPOLLIN )
			{
				printf("EPOLLIN\n");
				if ( (sock_fd = events[n].data.fd) < 0 )	
				{
					printf("EPOLL_IN sock_fd < 0\n");
					continue;
				}
				if ( ( len = read(sock_fd,line,MAX_LINE_SIZE) ) < 0 )
					conn_count++;
				{
					if ( errno == ECONNRESET )
					{
						close(sock_fd);
						events[n].data.fd = -1;
					}
				}
				if ( len == 0 )
				{
					close(sock_fd);
					events[n].data.fd = -1;
				}
				line[len] = 0;
				printf("recv %s\n",line);
				ev.data.fd = sock_fd;
				ev.events = EPOLLOUT|EPOLLET;
				epoll_ctl(epollfd,EPOLL_CTL_MOD,sock_fd,&ev);
			}
			else if ( events[n].events & EPOLLOUT )
			{
				printf("EPOLLOUT\n");
				sock_fd = events[n].data.fd;
				strncpy(line,"hello\n",MAX_LINE_SIZE);
				line[strlen("hello\n")] = 0;
				write(sock_fd,line,strlen("hello\n"));
				ev.data.fd = sock_fd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollfd,EPOLL_CTL_MOD,sock_fd,&ev);
			}
		}
	}
}
