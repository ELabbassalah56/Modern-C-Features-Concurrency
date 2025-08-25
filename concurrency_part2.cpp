// useing async to run a function in a separate thread
// and get a future to retrieve the result later this high level abstraction
// is easier to use than std::thread and it automatically manages the thread's
// lifetime
// it is part of the C++11 standard library and is defined in the <future> header
// it can be used to run any callable object (function, lambda, functor) in a separate thread
// and it returns a std::future object that can be used to retrieve the result of the callable
// when it is ready
// the callable is executed asynchronously (in a separate thread) and the future can be used
// to wait for the result or to check if the result is ready
// the std::async function takes two arguments: the first is the callable object to be executed
// the second is a variadic list of arguments to be passed to the callable object
// the std::async function returns a std::future object that can be used to retrieve the result
// of the callable object when it is ready
// the std::future object has a get() member function that can be used to retrieve the result
// of the callable object. If the result is not ready, the get() function will block until the result is ready
// the std::future object also has a wait() member function that can be used to wait for the result
// of the callable object without retrieving the result
// the std::future object also has a wait_for() member function that can be used to wait for the result
// of the callable object for a specified duration
// the std::future object also has a wait_until() member function that can be used to wait for the result
// of the callable object until a specified time point
// the std::async function can be used with different launch policies:
// std::launch::async: the callable object is executed in a new thread
// std::launch::deferred: the callable object is executed when the result is requested
// std::launch::async | std::launch::deferred: the callable object is executed in a new thread or when the result is requested
// if no launch policy is specified, the default is std::launch::async | std::launch::deferred
// example of using

#include <iostream>
#include <list>
#include <thread>
#include <string>
#include <chrono>
#include <future>

int Dowloader(int nums)
{
    std::cout << " Start Downloader with thread id : " <<std::this_thread::get_id() << std::endl;
    using namespace std::chrono_literals;
    int sum{};
    for (int i = 0; i < nums; ++i)
    {
        sum += i;
        std::cout << "Downloading file " << i << std::endl;
        std::this_thread::sleep_for(300ms);
    }
    return sum;
}

int main()
{

        auto future = std::async(/*std::launch::async,*/Dowloader, 10); // launch Dowloader in a separate thread
        // std::async(std::launch::async, Dowloader); // force to launch in a separate thread async
        // std::async(std::launch::deferred, Dowloader); // launch when result is requested synchronously in main thread
        // Dowloader();
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        std::cout << "Doing other work in main thread" << std::endl;
        std::cout << std::this_thread::get_id() << std::endl;
        int ret{};

        if(future.valid()){
            // future.wait(); // wait for the downloader to finish
            // auto status = future.wait_for(1s); // wait for 2 seconds
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
            ret = future.get(); // wait for the downloader to finish
        }
        std::cout << "Downloader finished return value is " << ret << std::endl;
        return EXIT_SUCCESS;
}