#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>

using namespace std;

struct WorkerData{
	bool done; //tells a worker to quit
};

void WorkerThread(WorkerData *workerData);

//describes a training session
struct NetworkTask{

	/****   network description    ****/
	vector <int> neuronsPerLayer;
	int id; //unique ID
	int RandomSeed;
	double TrainingVal;
	double IRB;
};

//static threadpool class
class Threadpool{
public:
	static void Begin(int nthreads);
	static void End();
	static void AddTask(NetworkTask *newTask);

	static condition_variable cond;
	static mutex taskQueueLock;
	static queue <NetworkTask *> taskQueue;
	static int nThreads;

	//threads
	static vector <thread *> workers;
	static vector <WorkerData> workerDatas;

	/****   training data  (same for every network execution)  ****/
	static vector < vector <double> > dataTable;
	static vector <bool> truths;
};
