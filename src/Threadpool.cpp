#include "Threadpool.h"
#include "NeuralNetwork.h"
#include <cstdlib>

void WorkerThread(WorkerData *workerData)
{
	//create and aquire lock. Need unique_lock wrapper so it can use a condition_variable
	std::unique_lock<std::mutex> lock(Threadpool::taskQueueLock);
	NetworkTask *currentTask;

	while (!(workerData->done))
	{
		while (Threadpool::taskQueue.empty()){
			if (workerData->done) { lock.unlock(); return; }
			Threadpool::cond.wait(lock); //wait for a signal from main thread
		}
		if (workerData->done) { lock.unlock(); return; }

		//grab a task from the queue
		currentTask = Threadpool::taskQueue.front();
		Threadpool::taskQueue.pop();

		//free the lock while we work
		lock.unlock();
		NeuralNetwork neuralNetwork;
		neuralNetwork.initializeNetwork(currentTask->neuronsPerLayer, currentTask->RandomSeed, currentTask->TrainingVal, currentTask->IRB);
		neuralNetwork.trainNet(Threadpool::dataTable, Threadpool::truths);
		delete currentTask; //free the memory

		//lock the queue
		lock.lock();
	}

	return;
}

 void Threadpool::Begin(int nthreads)
 {
	 nThreads = nthreads;
	 workers.resize(nThreads);
	 workerDatas.resize(nThreads);
	 for (int i = 0; i < nThreads; i++){
		 workerDatas[i].done = 0;
		 workers[i] = new thread(WorkerThread, &(workerDatas[i]));
	 }
 }

 void Threadpool::End()
 {
	 taskQueueLock.lock();
	 while (taskQueue.size()){ delete taskQueue.front(); taskQueue.pop(); } //empty the queue
	 for (int i = 0; i < workerDatas.size(); i++){
		 workerDatas[i].done = 1;
	 }
	 taskQueueLock.unlock();
	 cond.notify_all();
	 for (int i = 0; i < workers.size(); i++){
		 workers[i]->join();
	 }
 }

void Threadpool::AddTask(NetworkTask *newTask)
{
	taskQueueLock.lock();
	taskQueue.push(newTask);
	taskQueueLock.unlock();
}