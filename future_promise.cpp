/*
Futures and promises provide a high-level mechanism for asynchronous programming in
C++. They allow you to retrieve results from operations that might not have completed yet,
without blocking the calling thread until the result is needed.
Key Components:
1. Promise ( std::promise ): An object that "promises" to provide a result at some point in
the future.
2. Future ( std::future ): An object that refers to a result that will be available later.
3. Packaged Task ( std::packaged_task ): A wrapper for a callable that can be executed
asynchronously, with its result stored in a future.
4. Async ( std::async ): A high-level function to launch asynchronous tasks and get futures
for their results.
*/ 



#include<thread>
#include<future>
#include<iostream>
#include<list>
#include <chrono>

using namespace std::chrono_literals;

std::list<int> g_buffer;

void Consumed(std::future<std::list<int>> &arg_list)
{
    if(arg_list.valid())
        g_buffer = arg_list.get();

    for(const auto& item : g_buffer)
    {
        std::cout << "Consumed: " << item << std::endl;
    }
}

void Produced(std::promise<std::list<int>> &arg_list)
{
    std::list<int> l_buffer;
    for(int i = 0; i < 10; ++i)
    {
        std::cout << "Produced: " << i << std::endl;
        l_buffer.push_back(i);
        std::this_thread::sleep_for(100ms);
    }
    arg_list.set_value(l_buffer);    
}

int main()
{
    std::promise<std::list<int>> data;
    Produced(std::ref(data));
    std::future<std::list<int>> result = data.get_future();
    Consumed(result);


    // std::thread producer(Produced, std::ref(data));
    // std::thread consumer(Consumed, std::ref(result));

    // producer.join();
    // consumer.join();

    return EXIT_SUCCESS;
}