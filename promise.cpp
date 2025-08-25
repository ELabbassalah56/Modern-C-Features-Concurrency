// using promise use case which it is similer to end point which the promise creating the shared state
// then future is used to retrieve the result of the asynchronous operation
// promise is used to set the value of the shared state
// shared state is the data that is being transferred between the producer (promise) and consumer (future)
// promise can also propagate exceptions between threads
#include <iostream>
#include <list>
#include <thread>
#include <string>
#include <chrono>
#include <future>

using namespace std::chrono_literals;


std::promise<int> Dowloader(std::promise<int>& data)
{   
    std::promise<int> result;

    std::cout << " Start Downloader with thread id : " <<std::this_thread::get_id() << std::endl;
    int sum{};
    auto future = data.get_future();
    try{ 
        std::cout << "Waiting for data from main thread..." << std::endl;
        int nums = future.get();
        std::cout << "Received data from main thread: " << nums << std::endl;
        for (int i = 0; i < nums; ++i)
        {
            sum += i;
            std::cout << "Downloading file " << i << std::endl;
            std::this_thread::sleep_for(300ms);
            if(i == 5)
                throw std::runtime_error("Download failed");
        }
        result.set_value(sum);
    }catch(std::exception &ex)
    {
        result.set_exception(std::current_exception());
    }
    return result;

}

int main()
{

        std::promise<int> data; // used once to share and seting the data
        auto future = std::async(std::launch::async,Dowloader, std::ref(data)); 
        
        std::this_thread::sleep_for(2s);
        std::cout << "Doing other work in main thread" << std::endl;
        std::cout << std::this_thread::get_id() << std::endl;
        
        
        try{
        // throw std::runtime_error("Data not available!");
        data.set_value(10);
        
        if(future.valid()){
           auto status = future.wait_until(std::chrono::system_clock::now()+1s); // wait for 2 seconds
            switch (status)
            {
            case std::future_status::deferred:
                std::cout << "the task is synchronously" << std::endl;
                break;
            case std::future_status::timeout:
                std::cout << "the task is not finished yet" << std::endl;
                break;
            case std::future_status::ready:
                std::cout << "the task is finished" << std::endl;
            default:
                break;
            }
            auto ret = future.get(); // wait for the downloader to finish
            auto result =  ret.get_future();
            std::cout << "Downloader finished return value is " << result.get() << std::endl;
        }
    }
    catch(std::exception &ex)
    {
        std::cout << "[MAIN] caught exception: " << ex.what() << std::endl;
    }
    return EXIT_SUCCESS;
}


/*
What is a potential use for a thread's native_handle?

The native_handle() method of std::thread gives you access to the underlying OS-specific thread handle (e.g., a pthread_t on POSIX systems, or a HANDLE on Windows).

🔹 Potential uses:

To call platform-specific APIs that aren’t exposed through the standard C++ thread interface.

Examples:

Changing the thread’s priority.

Setting the thread’s CPU affinity (pinning a thread to specific cores).

Naming the thread using OS-specific functions.

Adjusting scheduling policies.
*/