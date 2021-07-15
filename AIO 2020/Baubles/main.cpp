#include <stdinc.h>
#include <defines.h>

using namespace Marble;

#undef Yield

Task<> doThing()
{
    for (int i = 0; i < 100; i++)
    {
        co_await Task<>::Yield();
    }
    std::cout << "hit dothing\n";
    co_return;
}
Task<> test()
{
    co_await Task<>::Yield();
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

using Bruh = unsigned long long;

__noinline int main()
{
    FileInput("tennisin.txt");
    std::cout << input << '\n';
}
