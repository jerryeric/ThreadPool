
#include "ThreadPool.h"
#include "WorkerThread.h"
#include <iostream>
using namespace std;

ThreadPool::ThreadPool(int numWorkers, int maxTasks):maxTasks(maxTasks),numWorkers(numWorkers), online(true)
{
	for(int i = 0; i < numWorkers; i++){
        WorkerThread *new_thread = new WorkerThread(*this);
        new_thread->start();
        workers.push_back(new_thread);
    }
}

ThreadPool::~ThreadPool() {

}

bool ThreadPool::schedule(Task *t)
{
    if(q.size() == maxTasks){
        return false;
    }
    else{
        lock_guard<mutex> lock(m);
        q.push(t);
        cvQueueNonEmpty.notify_one();
        return true;
    }
}

void ThreadPool::stop() {
	online = false;
	cvQueueNonEmpty.notify_all();
	for(auto& i:workers) {i->t.join(); }
}
