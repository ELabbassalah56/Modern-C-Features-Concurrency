/*

std::packaged_task wraps a callable object (function, lambda, etc.) so it can be executed
asynchronously with its result stored in a future
// Step 1: Create a packaged_task wrapping the function
    std::packaged_task<int(int)> task(square);

    // Step 2: Get the future
    std::future<int> fut = task.get_future();

    // Step 3: Run the task in a thread
    std::thread t(std::move(task), 10);

    // Step 4: Get the result
    std::cout << "Result: " << fut.get() << "\n"; // prints 100

    t.join();



Key points about std::packaged_task :
• It wraps a callable object and allows you to get a future for its result
• It doesn't execute the callable automatically; you need to call it explicitly
• It's useful when you want to control when and where a task is executed
• It's typically used with thread pools or task systems
*/

#include <future>
#include <iostream>
#include <thread>
#include <cmath>
#include <vector>


bool is_prime(int n) {

    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n%2 == 0 || n%3 == 0) return false;
    for (int i = 5; i*i <= n; i+=6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int count_prime(int start, int end)
{
    int count = 0;
    for (int i = start; i <= end; ++i) {
        if (is_prime(i)) {
            ++count;
        }
    }
    return count;
}




int main()
{

    const int num_tasks = 4;
    const int range_size = 250000;
    // std::packaged_task<int(int, int)> task(count_prime);
    std::vector<std::packaged_task<int(int, int)>> tasks;
    std::vector<std::future<int>> futures;
    //create tasks
    for(int i=0;i<num_tasks;i++)
    {
        int start = i * range_size + 1;
        int end = (i + 1) * range_size;
        std::packaged_task<int(int,int)> task(count_prime);
        futures.push_back(task.get_future());
        tasks.push_back(std::move(task));
    }
    std::vector<std::thread> threads;
    // Launch tasks in separate threads
    for (int i = 0; i < num_tasks; ++i) {
        int start = i * range_size + 1;
        int end = (i + 1) * range_size;
        threads.emplace_back([&tasks, i, start, end]() {
        tasks[i](start, end);
        });
    }
    // std::future<int> fut = task.get_future();
    // std::thread t{std::move(task),1,100};
    // if(fut.valid())
    // {
    //     int x = fut.get();
    //     std::cout << "Result: " << x << "\n";
    // }

    // t.join();

    for (auto& thread : threads) {
        thread.join();
    }
    
    // Collect and sum the results
    int total_primes = 0;
    
    for (auto& future : futures) {
        total_primes += future.get();
    }
    
    std::cout << "Total primes found: " << total_primes << std::endl;
    return 0;
}