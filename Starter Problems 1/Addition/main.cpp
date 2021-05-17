#include <stdinc.h>
#include <defines.h>

using namespace Marble;

int main()
{
    FileInput("addin.txt");

    std::vector<string> twoNums = input.split(" ");
    string out = string::format("%u", twoNums.begin()->toIntegralType<uint64>() + (++twoNums.begin())->toIntegralType<uint64>());

    FileOutput("addout.txt", out);
}
