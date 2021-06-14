#include <stdinc.h>
#include <defines.h>
#undef Yield

using namespace Marble;
using namespace std::literals::chrono_literals;

void run()
{
    ThreadPool::pushThreads(ThreadPool::availableHardwareThreads());

    Parallel::forRange(420, 1337, [](size_t i)
    {
        std::ostringstream bruh;
        bruh << i << "\n";
        std::cout << bruh.str();
    });
    std::cout << "\n";
    Parallel::forRangeBreakable(420, 1337, [](size_t i, auto state)
    {
        std::ostringstream bruh;
        bruh << i << "\n";
        std::cout << bruh.str();
        if (state.breakRequested())
            std::cout << "bruhbruh\n";
        if (i == 421)
            state.breakAll();
    });
    std::vector<char> cs(1200, 'a');
    Parallel::forEach<char>(cs, [](auto c) -> void
    {
        std::ostringstream bruh;
        bruh << c << "\n";
        std::cout << bruh.str();
    });

    ThreadPool::killAllThreads();
}
int main()
{
    run();
}
