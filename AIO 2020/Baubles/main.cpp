#include <stdinc.h>
#include <defines.h>

using namespace Marble;

struct test
{
int i = 0;
    test()
    {
        std::cout << "ctor\n";
    }
    ~test()
    {
        std::cout << "dtor\n";
    }
};

test* testfunc()
{
    test* ptr = nullptr;

    try
    {
        lowLevelExceptionsSectionBegin

        test tsts;
        tsts.i = 69;
        ptr = &tsts;

        *(int*)0 = 0;

        lowLevelExceptionsSectionEnd
    }
    catch (Marble::LowLevelException& ex)
    {
        std::cout << ex.what() << '\n';
    }

    return ptr;
}
int main()
{
    while (true)
    {
        try
        {
            struct __test
            {
                char* testarr;
                __test()
                {
                    this->testarr = new char[262144];
                }
                ~__test()
                {
                    delete[] this->testarr;
                }
            };
            
            lowLevelExceptionsSectionBegin

            volatile char __c[32768]{ 0 };
            std::cout << __c << '\n';
            
            __test arr;
            printf("%p\n", arr.testarr);

            std::cout << testfunc()->i << '\n';

            *(int*)0 = 0;

            lowLevelExceptionsSectionEnd
        }
        catch (Marble::LowLevelException& ex)
        {
            std::cout << ex.what() << '\n';
        }
    }
}
