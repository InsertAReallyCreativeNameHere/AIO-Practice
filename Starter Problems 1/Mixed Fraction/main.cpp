#include <stdinc.h>
#include <defines.h>

using namespace Marble;

int main()
{
    FileInput("mixin.txt");

    auto twoNums = input.split(" ");
    uint64 n = twoNums[0].toIntegralType<uint64>();
    uint64 d = twoNums[1].toIntegralType<uint64>();

    uint64 wholePart = n / d;
    uint64 fractionPart = n - d * (n / d);

    StringBuilder builder;
    builder.append(string::format("{}", wholePart));
    if (fractionPart != 0)
        builder.append(string::format(" {}/{}", fractionPart, d));
    string out = builder.construct();

    FileOutput("mixout.txt", out);
}
