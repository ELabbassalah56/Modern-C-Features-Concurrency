/*
    std::promise allows you to set a value or exception that will be retrieved through an
    associated future:
    Key points about std::promise :
• It's the "producer" end of the promise-future pair
• You can set a value with set_value() or an exception with set_exception()
• Each promise is associated with exactly one future
• A promise can only be fulfilled once
• Promises are typically moved into threads, as they're not copyable
*/

#include<iostream>
#include<thread>
#include<future>
#include<vector>


void Producer(std::promise<std::vector<int>> &vec)
{
    std::vector<int> v;
    
    try
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for(int i=0;i<10;i++)
        {
            v.push_back(i);
        }

        vec.set_value(v);

    }catch(...)
    {
        vec.set_exception(std::current_exception());
    }

    
}
void Producer_with_error(std::promise<std::vector<int>> &vec)
{
    
    std::vector<int> v;
    
    try
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        throw std::runtime_error("Producer stuck!");
        for(int i=0;i<10;i++)
        {
            v.push_back(i);
        }

        vec.set_value(v);

    }catch(...)
    {
        vec.set_exception(std::current_exception());
    }

}


int main(){

    std::promise<std::vector<int>> vec1;
    std::future<std::vector<int>> fut1 = vec1.get_future();
    std::thread t1(Producer, std::ref(vec1));
    
    std::promise<std::vector<int>> vec2;
    std::future<std::vector<int>> fut2 = vec2.get_future();
    std::thread t2(Producer_with_error, std::ref(vec2));

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<< "Main thread Doing work" << std::endl;
    try{
        fut1.wait();
        if(fut1.valid())
        {
            auto v = fut1.get();
            for(auto &i : v)
                std::cout<< i << " ";
            std::cout<< std::endl;
        }

    }catch(std::exception &ex)
    {
        std::cout<< "Error occurred while getting future result: " << ex.what() << std::endl;
    }
    try
    {
        fut2.wait();
        if(fut2.valid())
        {
            auto v = fut2.get();
            for(auto &i : v)
                std::cout<< i << " ";
            std::cout<< std::endl;
        }

    }catch(std::exception &ex)
    {
        std::cout<< "Error occurred while getting future result: " << ex.what() << std::endl;
    }
    t1.join();
    t2.join();

    return EXIT_SUCCESS;
}