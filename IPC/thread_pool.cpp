#ifndef THREAD_POOL_CPP
#define THREAD_POOL_CPP


#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <iostream>
#include <unistd.h>


// [?] why we use queue in this case?

class ThreadPool
{
    public:
        void Start(int thread_num);
        void EnqueueJob(std::function<void()> &job);
        void Stop();
        bool IsBusy();

    private:
        void ThreadFunc(int thread_id);
        bool shoudlTerminate = false;
        std::mutex mu;
        std::condition_variable cond;
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> job_queue;
};

void ThreadPool::ThreadFunc(int thread_id)
{
    while(true)
    {
        std::function<void()> job;
        std::unique_lock<std::mutex> lock(mu);
        std::cout << "thread_num: " << thread_id << " is working" << std::endl;
        cond.wait(lock, [this] {return !job_queue.empty() || shoudlTerminate;});
        if(shoudlTerminate)
        {
            return;
        }
        else if(!job_queue.empty())
        {
            job = job_queue.front();
            job_queue.pop();
        }
        lock.unlock();  // Unlock the mutex before executing the job
        job();  // Execute the job
        std::cout << "thread_id: " << thread_id << " done the jobbb!!!" << std::endl;
    }
};

void ThreadPool::Start(int thread_num)
{
    for(int i = 0; i < thread_num; i++)
    {
        threads.push_back(std::thread(&ThreadPool::ThreadFunc, this, i));
    }
};

void ThreadPool::EnqueueJob(std::function<void()> &job)
{
    std::lock_guard<std::mutex> lock(mu);
    job_queue.push(job);
    cond.notify_all();
};

bool ThreadPool::IsBusy()
{
    bool res;
    {
        std::lock_guard<std::mutex> lock(mu);
        res = !job_queue.empty();
    }

    std::cout << "res: " << res << "\n";

    return res;
};

void ThreadPool::Stop()
{
    {
        std::lock_guard<std::mutex> lock(mu);
        shoudlTerminate = true;
    }
    cond.notify_all();

    for(auto i = 0; i < threads.size(); i++)
    {
        std::cout << "end the stopppp";
        threads[i].join();
        std::cout << "end the stopppp";
    }
    threads.clear();
    std::cout << "end the stopppp";
}

void print_number(int num)
{
    sleep(1);
    // std::unique_lock<std::mutex> lock(mu_log);
    std::cout << "print number: " << num << std::endl;
}

// int main()
// {
//     ThreadPool pool;
//     pool.Start(5);

//     // waite for start
//     sleep(1);

//     std::function<void()> job;
//     for(int i = 0; i < 5; i++)
//     {
//         job = std::bind(print_number, i);
//         pool.EnqueueJob(job);
//     }

//     while(pool.IsBusy())
//     {
//     }

//     pool.Stop();

//     return 0;
// }



#endif




