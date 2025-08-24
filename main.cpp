#include <iostream>
#include <list>
#include <thread>
#include <string>
std::list<int> g_Data;
const long MAX_DATA_SIZE = 5000000;
// How many instances of String are created, copied, assigned, and destroyed?
class String
{
public:
    static int global_id; // total count of objects ever created
    int id;               // unique ID for this object

    String() : id(++global_id)
    {
        std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__
                  << "] String Constructor (id=" << id << ")\n";
    }

    String(const String &other) : id(++global_id)
    {
        std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__
                  << "] String Copy Constructor (id=" << id
                  << " from id=" << other.id << ")\n";
    }

    String &operator=(const String &other)
    {
        ++global_id;
        id = global_id;
        std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__
                  << "] String Copy Assignment Operator (id=" << id
                  << " = from id=" << other.id << ")\n";
        return *this;
    }

    ~String()
    {
        std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__
                  << "] String Destructor (id=" << id << ")\n";
    }
};

int String::global_id = 0;
// if i do const String &file, then no copy constructor is called
// and we need to user std::ref(file)/std::cref(file) in main thread
void Download(const String  &file)
{
    std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__ << "] Enter Download() with file id=" << file.id << "\n";
    for (int i = 0; i < MAX_DATA_SIZE; ++i)
    {
        g_Data.push_back(i);
    }
    std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__ << "] Exit Download() with file id=" << file.id << "\n";
}

// thread constructor is varaiadic template
// template <class... Args> explicit thread(Args&&... args);
// template <class F, class... Args> explicit thread(F&& f, Args&&   ... args);
// The first argument is the function to be executed by the thread, and the remaining arguments are the arguments to be passed to that function.
// If the function takes no arguments, you can simply pass the function name without any additional arguments.
// If the function takes arguments, you can pass them after the function name.
// Example:
// std::thread t(FunctionName, arg1, arg2, ...);
// If the function is a member function of a class, you need to pass the object (or a pointer/reference to the object) as the first argument, followed by the member function pointer and any additional arguments.
// Example:
// std::thread t(&ClassName::MemberFunction, &object, arg1, arg2, ...);
// If the function is a lambda expression, you can pass the lambda directly.
// Example:
// std::thread t([](arg1, arg2, ...) { /* function body */ }, arg1, arg2, ...);
// If you want to pass arguments by reference, you need to use std::ref to wrap the arguments.
// Example:
// std::thread t(FunctionName, std::ref(arg1), std::ref(arg2), ...);
// If you want to pass a callable object (like a functor), you can pass the object directly.
// Example:
// std::thread t(CallableObject, arg1, arg2, ...);
// If you want to pass a function pointer, you can pass the pointer directly.
// Example:
// std::thread t(FunctionPointer, arg1, arg2, ...);
// If you want to pass a member function pointer, you need to use the syntax &ClassName::MemberFunction.
// Example:
// std::thread t(&ClassName::MemberFunction, &object, arg1, arg2, ...);
// If you want to pass a static member function, you can pass it like a regular function.
// Example:
// std::thread t(&ClassName::StaticMemberFunction, arg1, arg2, ...);
// If you want to pass a function with default arguments, you can simply omit the arguments you want to use the default values for.
// Example:
// std::thread t(Download, arg1, arg2, ...);
/*
Why does this compile on win passing by ref without using std::ref?
MSVC (Visual Studio) does something non-standard:
It lets you pass a normal variable (file) to a std::thread function that expects a non-const lvalue reference (std::string&).
It essentially binds the reference, even though the standard requires you to explicitly use std::ref(file).

GCC / Clang (on Linux) are stricter and follow the C++ standard more closely. They will give the error you saw, because std::thread copies its arguments by default, and a temporary cannot bind to a non-const reference. 
*/

/* section2 */
/* 
    1- Thread Synchronization (std::mutex) 
*/
int main()
{
    String file1{}; // 1st String Constructor
    std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__
              << "] Main thread started.\n";
    std::thread t(Download, std::cref(file1) /*2nd String Copy Constructor*/);
    // t.detach();
    std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__
              << "] Main thread finished.\n";
    if (t.joinable())
    {
        std::cout << "[Line " << __LINE__ << " in " << __FUNCTION__
                  << "] Joining the thread.\n";
        t.join();
    }
    // system("read -p 'Press Enter to continue...' var");
    return EXIT_SUCCESS;
}