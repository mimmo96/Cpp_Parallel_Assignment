#include <iostream>
#include <thread>
#include <vector>
#include <future>  
#include <queue>
#include "utils.cpp"

using namespace std;

class threadpool{

    private:
        mutex queue_mutex;                      // Prevents data races to the job queue
        condition_variable mutex_condition;     // Allows threads to wait on new jobs or termination 
        bool should_terminate = false;          // Tells threads to stop looking for jobs
        queue<function<int()>> jobs;
        vector<thread> threads;
        int nw;

        void threadLoop(){
            while (true) {
                std::function<int()> job;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    mutex_condition.wait(lock, [this] {
                        return !this->jobs.empty() || this->should_terminate;
                    });
                    if (this->should_terminate) {
                        return;
                    }
                    job = this->jobs.front();
                    this->jobs.pop();
                }
                int res = job();    //esegui il lavoro
                cout << "do job:" << res << endl;
            }
        }

    public:

        threadpool(int nw){
            //const int num_threads = thread::hardware_concurrency(); // Max # of threads the system supports
    
            for (int i = 0; i < nw; i++) {
                threads.emplace_back([this](){ this->threadLoop(); });
            }
        }


        bool is_busy(){
            bool poolbusy;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                poolbusy = jobs.empty();
            }
            return poolbusy;
        }

        void addJob(const function<int()>& job){
            {
                unique_lock<std::mutex> lock(queue_mutex);
                jobs.push(job);
            }
            mutex_condition.notify_one();
        }

        void Stop(){
            {
                std::unique_lock<std::mutex> lock(this->queue_mutex);
                this->should_terminate = true;
            }
            mutex_condition.notify_all();
            for (std::thread& active_thread : threads) {
                active_thread.join();
            }
            threads.clear();
        }

};