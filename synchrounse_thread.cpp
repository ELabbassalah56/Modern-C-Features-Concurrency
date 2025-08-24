#include <iostream>
#include <list>
#include <thread>
#include <string>
#include <mutex>

std::list<int> g_Data;
const long MAX_DATA_SIZE = 10;
std::mutex g_Mutex;
void Download(const std::string  &file)
{
    pthread_setname_np(pthread_self(), "DownloadThread");
    for (int i = 0; i < MAX_DATA_SIZE; ++i)
    {
        // g_Mutex.lock();
        std::lock_guard<std::mutex> lock(g_Mutex);
        g_Data.push_back(i);
        // this is to simulate an error and see how it affects the mutex locking
        // uncomment the below code to see the effect
        // will cause a deadlock because the mutex will not be unlocked
        // and the other thread will be waiting forever to acquire the lock
        if(i == MAX_DATA_SIZE / 2)
        {
            // simulate some error
            // throw std::runtime_error("Some error happened");
            return;
        }
        // g_Mutex.unlock();
    }
}
void Download2(const std::string  &file)
{
    pthread_setname_np(pthread_self(), "Download2Thread");
    for (int i = 0; i < MAX_DATA_SIZE; ++i)
    {
        std::lock_guard<std::mutex> lock(g_Mutex);
        // g_Mutex.lock();
        g_Data.push_back(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // g_Mutex.unlock();
    }
}

/*
    to avoid the issue of forgetting to unlock the mutex we can use std::lock_guard
    which will automatically unlock the mutex when it goes out of scope
    this is a better way to handle mutex locking and unlocking
    because it will ensure that the mutex is always unlocked even if an exception is thrown
    or if we return from the function early
    we can use it like this:
    {
        std::lock_guard<std::mutex> lock(g_Mutex);
        // critical section
    } // mutex is automatically unlocked here   
    this is called RAII (Resource Acquisition Is Initialization) idiom
    this name is used because the resource (mutex) is acquired (locked) 
    when the object (lock) is created and released (unlocked) when the object goes out of scope

    this is not the better way to handle the resource locking and unlocking
    and this means if the cpu is idle the waiting thread will not get the chance to run
    and this will cause a performance issue . so in general the locking should be avoided and should be found other 
    ways to handle the resource sharing between threads.
    so we can use different data structure for each function then merge them together at the end

    */

int main()
{
    /* 
        the output will doing race condition which our two threads reaced to accessing the data 
        at the same time and this will cause data corruption
        to solve this issue we need to use mutex to lock the data when one thread is accessing it 
        and unlock it when it is done
    */
    std::string file1{"file1"};
    std::string file2{"file2"};
    std::thread t1(Download, std::cref(file1) );
    std::thread t2(Download2, std::cref(file2));
    std::cout << "thread1 id: "<< t1.get_id() << " thread2 id:" << t2.get_id() << "\n";
    t1.join();
    t2.join();
    std::cout << std::thread::hardware_concurrency() << " concurrent threads are supported.\n";
    std::cout << "Data size: " << g_Data.size() << "\n";
    return EXIT_SUCCESS;
}