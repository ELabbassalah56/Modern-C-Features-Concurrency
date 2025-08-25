#include <future>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;



void process()
{
    for(int i=0; i<10;i++ )
    {
        std::cout<< "Processing item [" << std::this_thread::get_id() << "] : " << i << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

/*
In this example:
• std::async launches compute_value() asynchronously and returns a future
• The main thread continues execution while the computation runs in the background
• When we call get() on the future, it blocks until the result is available
• Once the result is retrieved, the future becomes invalid (you can only call get() once)
*/



int main()
{

    auto fut = std::async(std::launch::async, process);
    std::this_thread::sleep_for(5s);
    for(int i = 10; i>=0; --i)
    {
        std::cout<< "Main Thread working [" << std::this_thread::get_id() << "] : " << i << std::endl;
    }
    if(fut.valid())
        fut.get();

    return EXIT_SUCCESS;
}