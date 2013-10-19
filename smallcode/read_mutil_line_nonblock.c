#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 8196
#define MAX_NUM_LINE     1000
typedef struct
{
	int num;
	char content[MAX_NUM_LINE][LINE_BUFFER_SIZE];
}Lines;

typedef void deal_lines_fun (Lines *pLines);

void fun(Lines *pLines)
{
	static int count=0;
	fprintf(stderr,"lines: %d\n",count+=pLines->num);
	int i;
	for ( i=0; i<pLines->num; ++i )
	{
		printf("%s\n",pLines->content[i]);
	}
}

int setNonBlock(int fd)
{
	int ret = fcntl(fd,F_SETFL,fcntl(fd,F_GETFL)|O_NONBLOCK);
	if ( ret == -1 )
		perror("setNonBlock:");
	return ret;
}

/**
* 用非阻塞方式从fd中读取多行
*/
int read_mutil_line_nonblock(int fd,deal_lines_fun fun)
{
	//设置fd非阻塞
	if ( setNonBlock(fd) == -1 )
		return -1;

	Lines lines; //存储读取到的行
	char buf[LINE_BUFFER_SIZE]={0};      
	char tmpBuf[LINE_BUFFER_SIZE]={0};
	int ret;
	int tmp_len=0;

	while ( 1 )
	{
		if ( strlen(tmpBuf) == LINE_BUFFER_SIZE -1 )
		{//一行超过最大值，丢弃
			printf("tmpBuf=%s len=%d, drop\n",tmpBuf,strlen(tmpBuf));
			tmpBuf[0] = 0;
		}
		tmp_len = strlen(tmpBuf);
		lines.num=0;
		strncpy(buf,tmpBuf,LINE_BUFFER_SIZE-1);
		tmpBuf[0]=0;
		ret = read(fd,buf+tmp_len,LINE_BUFFER_SIZE-tmp_len-1);
		if ( ret >0 )
		{//读取到数据
			char *p = buf;
			char *q = NULL;
			int i=0;

			buf[ret+tmp_len] = 0;
			while ( 1 )
			{
				q = p;
				p = strstr(p,"\n");
				if ( p != NULL )
				{//读到一个回车符号
					*p++=0; //将回车符号换成'\0',且p指向下一个字符
				}

				if ( p == NULL )
				{//读到最后面了，且最后一行没有回车符号
				    strncpy(tmpBuf,q,LINE_BUFFER_SIZE-1);
					//fprintf(stderr,"tmpBuf=%s\n",tmpBuf);
					break;
				}
				else
				{//
					strncpy(lines.content[i++],q,LINE_BUFFER_SIZE-1);
					lines.num++;
					if (  *p == 0)
					{//读到最后面了，且最后一行有回车符号
						break;
					}
				}
			}
			fun(&lines);
		}
		else if (ret == 0 )
		{//没数据了
			if ( strcmp(tmpBuf,"") != 0 )
			{
				strncpy(lines.content[0],tmpBuf,LINE_BUFFER_SIZE-1);
				lines.num=1;
				fun(&lines);
			}
			break;
		}
		else if ( ret == -1 && errno == EAGAIN )
		{//等待数据
			sleep(1);
			fprintf(stderr,"EAGAIN\n");
			continue;
		}
		else
		{//发生错误
			perror("read_mutil_line_nonblock:");
			return -1;
		}
	}
	return 0;
}

int main()
{
	read_mutil_line_nonblock(0,fun);
	return 0;
}
