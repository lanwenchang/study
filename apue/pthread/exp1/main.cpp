/**************************************************
* 线程相关函数
*	pthread_create
*	pthread_join 
*	pthread_exit
****************************************************/
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
using namespace std;

struct Msg
{
	int ret;
	string s;

};

void* fun(void* arg)
{
	Msg *pMsg = (Msg*) arg;
	pid_t pid;
	pid = getpid();
	pthread_t tid = pthread_self();

	cout<<"pid="<<pid<<endl;
	cout<<"tid="<<tid<<endl;

	pMsg->ret = 1;
	pMsg->s = "hello";

	pthread_exit((void*)pMsg);
	//return (void*)pMsg;
}

int main()
{
	//创建线程，并出入参数
	Msg msg;
	pthread_t tid;
	int err = pthread_create(&tid,NULL,fun,(void*)&msg);
	if ( err != 0 )
		cout<<strerror(err)<<endl;

	//接受线程退出时的返回值
	Msg* pMsg;
	err = pthread_join(tid,(void**)&pMsg);
	if ( err != 0  )
		cout<<strerror(err)<<endl;

	cout<<"ret="<<pMsg->ret<<" s="<<pMsg->s<<endl;
	cout<<"main exit"<<endl;
	return 0;
}

