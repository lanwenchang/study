/*****************************************************
* 线程同步
*	互斥锁
*	pthread_mutex_t
*	pthread_mutex_init
*	pthread_mutex_destroy
*	pthread_mutex_lock
*	pthread_mutex_unlock
****************************************************/
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <string>
#include <deque>
using namespace std;

deque<int> vt;
const unsigned int MAX_SIZE=10;
pthread_mutex_t mutex;

//生产者
void* generator(void* arg)
{
	int i=0;
	while(1)
	{
		if(vt.size() < MAX_SIZE)
		{
			pthread_mutex_lock(&mutex);
			cout<<"generator i="<<i<<endl;
			vt.push_back(i++);
			pthread_mutex_unlock(&mutex);
		}
		sleep(1);
	}
	return (void*)0;
}

//消费者
void* comsumer(void* arg)
{
	while(1)
	{
		if ( vt.size() > 0 )
		{
			pthread_mutex_lock(&mutex);
			int i = vt.front();
			cout<<"comsumer i="<<i<<endl;
			vt.pop_front();
			pthread_mutex_unlock(&mutex);
		}
		sleep(1);
	}
	return (void*)0;
}

int main()
{
	pthread_mutex_init(&mutex,NULL);

	pthread_t t1,t2;
	int err=0;
	err = pthread_create(&t1,NULL,generator,(void*)NULL);
	if ( err != 0 )
	{
		cout<<"main create thread generator err:"<<strerror(err)<<endl;
		return 0;
	}

	err = pthread_create(&t2,NULL,comsumer,(void*)NULL);
	if ( err != 0 )
	{
		cout<<"main create thread comsumer err:"<<strerror(err)<<endl;
		return 0;
	}

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}
