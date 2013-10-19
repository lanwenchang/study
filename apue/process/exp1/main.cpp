#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
	pid_t pid = fork();
	if ( pid == -1 )
	{
		printf ("fork fail\n");
		exit(-2);
	}

	if ( pid == 0  )
	{
		printf("child pid=%d ppid=%d\n",getpid(), getppid());
		exit(2);
	}
	else if ( pid > 0 )
	{
		int status=-1;
		int cpid = wait(&status);
		if ( cpid == -1 )
		{
			printf("wait err\n");
			return 0;
		}
		if ( WIFEXITED(status) )
		{
			printf("chiled exit\n");
		}
		else
		{
			printf("child else\n");
		}
		printf("wait cpid=%d,status=%d\n",cpid,status);
		printf("parent pid=%d cpid=%d\n", getpid(), pid);
	}
	return 0;
}
