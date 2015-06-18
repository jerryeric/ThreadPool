#include "ThreadPool.h"
#include "WorkerThread.h"
#include<thread>
#include<iostream>
#include<mutex>
using namespace std;

WorkerThread::WorkerThread(ThreadPool &_pool):pool(_pool){

}

WorkerThread::~WorkerThread() {

}


void WorkerThread::start()
{
	// Create the thread and make it run the run() function
	t = thread(&WorkerThread::run, this);
	//t.detach();
}

void WorkerThread::run()
{
    Task *task;
    while(pool.online){
        {   
            unique_lock<mutex> lock(pool.m);
	    pool.cvQueueNonEmpty.wait(lock, [this]() {return !pool.q.empty() || !pool.online;});
	    if (!pool.online) break;
            task = pool.q.front();
            pool.q.pop();
        }
        task->run();
    }
}
