/*
This example demonstrates three ways to wait for futures:
• wait() : Blocks until the result is available
• wait_for(duration) : Waits for a specified duration, returns a status
• wait_until(time_point) : Waits until a specified time point, returns a status
The status can be:
• std::future_status::ready : The result is ready
• std::future_status::timeout : The wait timed out
• std::future_status::deferred : The task hasn't started yet (for deferred execution)
 */


#include <future>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;



int process(int wait)
{
    std::this_thread::sleep_for(std::chrono::seconds(wait));
    return wait * 10;
}




int main()
{
    std::future<int> fut1 = std::async(std::launch::async, process, 1);
    auto fut2 = std::async(std::launch::async, process,2);
    auto fut3 = std::async(std::launch::async, process,3);
    std::this_thread::sleep_for(5s);
        fut1.wait();
    if(fut1.valid())
       std::cout<<"Future 1: " << fut1.get() << std::endl;

    if(fut2.wait_for(std::chrono::seconds(2)) == std::future_status::ready)
    {
       std::cout<<"Future 2: " << fut2.get() << std::endl;
    }
    else
    {
        std::cout << "Future 2 is not ready yet, but we'll wait for it" <<std::endl;
        fut2.wait();
    }
    for(int i = 10; i>=0; --i)
    {
        std::cout<< "Main Thread working [" << std::this_thread::get_id() << "] : " << i << std::endl;
    }

    auto duration = std::chrono::system_clock::now() - std::chrono::seconds(2);
    if(fut3.wait_until(duration) == std::future_status::ready)
    {
       std::cout<<"Future 3: " << fut3.get() << std::endl;
    }
    else
    {
        std::cout << "Future 3 is not ready yet, but we'll wait for it" <<std::endl;
        fut3.wait();
    }

    return EXIT_SUCCESS;
}