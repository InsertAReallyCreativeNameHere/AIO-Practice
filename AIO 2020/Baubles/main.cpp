#include <stdinc.h>
#include <defines.h>

void test()
{
    std::atomic<bool>* completed = new std::atomic<bool>[std::thread::hardware_concurrency()] { false };
    auto begin = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
    {
        std::thread([=]()
        {
            while (!Marble::ExceptionHandler::ctrlCInterruptThrown())//std::chrono::high_resolution_clock::now() - begin < std::chrono::milliseconds(8000))
            {
                try
                {
                    lowLevelExceptionsSectionBegin();

                    int i = 5;
                    int j = 0;
                    int k = i / j;

                    lowLevelExceptionsSectionEnd();
                }
                catch (Marble::LowLevelException& ex)
                {
                    //std::cout << ex.what() << std::endl;
                }
            }
            completed[i] = true;
        }).detach();
    }
    while (true)
    {
        bool allCompleted = true;
        for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
        {
            if (completed[i] == false)
                allCompleted = false;
        }
        if (allCompleted)
            break;
    }
    delete[] completed;
}
void test2()
{
    std::atomic<bool>* completed = new std::atomic<bool>[std::thread::hardware_concurrency()] { false };
    auto begin = std::chrono::high_resolution_clock::now();
    /*for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
    {
        std::thread([=]()
        {*/
            while (!Marble::ExceptionHandler::ctrlCInterruptThrown())//std::chrono::high_resolution_clock::now() - begin < std::chrono::milliseconds(8000))
            {
                try
                {
                    lowLevelExceptionsSectionBegin();

                    volatile int i = 5;
                    volatile int j = 0;
                    volatile int k = i / j;

                    lowLevelExceptionsSectionEnd();
                }
                catch (Marble::LowLevelException& ex)
                {
                    std::cout << ex.what() << std::endl;
                }
            }
            /*completed[i] = true;
        }).detach();
    }
    while (true)
    {
        bool allCompleted = true;
        for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
        {
            if (completed[i] == false)
                allCompleted = false;
        }
        if (allCompleted)
            break;
    }*/
    delete[] completed;
}
int main()
{
    test2();
}