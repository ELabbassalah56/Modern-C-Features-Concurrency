#include <iostream>
#include <list>
#include <thread>
#include <string>
#include <mutex>

std::list<int> g_Data;
const long MAX_DATA_SIZE = 10000;
std::mutex g_Mutex;
void Download(const std::string  &file)
{
    for (int i = 0; i < MAX_DATA_SIZE; ++i)
    {
        g_Mutex.lock();
        g_Data.push_back(i);
        g_Mutex.unlock();
    }
}
void Download2(const std::string  &file)
{
    for (int i = 0; i < MAX_DATA_SIZE; ++i)
    {
        g_Mutex.lock();
        g_Data.push_back(i);
        g_Mutex.unlock();
    }
}



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
    t1.join();
    t2.join();
    std::cout << "Data size: " << g_Data.size() << "\n";
    return EXIT_SUCCESS;
}