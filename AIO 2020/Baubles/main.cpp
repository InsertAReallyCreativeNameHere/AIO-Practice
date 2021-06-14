#include <stdinc.h>
#include <defines.h>

using namespace Marble;

Task<> test()
{
    std::cout << "bruh\n";
    co_await WaitUntilSecondsPassed(2.0f);
    std::cout << "bruh\n";
}

void run()
{
    test();
}
int main()
{
    run();
}
