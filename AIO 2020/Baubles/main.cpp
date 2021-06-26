#include <stdinc.h>
#include <defines.h>

using namespace Marble;

Task<> doThing()
{
    for (int i = 0; i < 100; i++)
    {
        co_await Task<>::yield();
    }
    std::cout << "hit dothing\n";
    co_return;
}
Task<> test()
{
    co_await Task<>::yield();
    Task<> test = doThing();
    std::cout << "hit test\n";
    co_await test;
    co_return;
}
void notcoro()
{
    for (int i = 0; i < 400; i++)
        test().wait();
    std::cout << "hit main\n";
}
__noinline int main()
{
    std::cout << "indicator entry\n";
    notcoro();
    std::cout << "indicator exit\n";
    std::this_thread::sleep_for(10000ms);
}
