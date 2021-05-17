#include <stdinc.h>
#include <defines.h>

using namespace Marble;

int main()
{
    FileInput("bendin.txt");

    struct Vector4
    {
        uint64 values[4];

        auto x1()
        {
            return values[0];
        }
        auto x2()
        {
            return values[2];
        }
        auto y1()
        {
            return values[3];
        }
        auto y2()
        {
            return values[1];
        }
    };
    auto twoLines = input.split("\n");
    auto firstRect = twoLines[0].split(" ");
    auto secondRect = twoLines[1].split(" ");
    Vector4 rec1 { firstRect[0].toIntegralType<uint64>(), firstRect[1].toIntegralType<uint64>(), firstRect[2].toIntegralType<uint64>(), firstRect[3].toIntegralType<uint64>() };
    Vector4 rec2 { secondRect[0].toIntegralType<uint64>(), secondRect[1].toIntegralType<uint64>(), secondRect[2].toIntegralType<uint64>(), secondRect[3].toIntegralType<uint64>() };
    
    constexpr auto calculateOverlap = [](uint64 p1x1, uint64 p1x2, uint64 p2x1, uint64 p2x2)
    {
        return p1x2 - p1x1 + p2x2 - p2x1 - (p2x2 - p1x1);
    };

    std::cout << (rec1.x2() - rec1.x1()) * (rec1.y2() - rec1.y1()) << "  " << (rec2.x2() - rec2.x1()) * (rec2.y2() - rec2.y1()) << "  " << calculateOverlap(rec1.y1(), rec1.y2(), rec2.y1(), rec2.y2()) * calculateOverlap(rec1.x1(), rec1.x2(), rec2.x1(), rec2.x2()) << std::endl;
    string out = string::format("{}", (rec1.x2() - rec1.x1()) * (rec1.y2() - rec1.y1()) + (rec2.x2() - rec2.x1()) * (rec2.y2() - rec2.y1()) - calculateOverlap(rec1.y1(), rec1.y2(), rec2.y1(), rec2.y2()) * calculateOverlap(rec1.x1(), rec1.x2(), rec2.x1(), rec2.x2()));
    FileOutput("bendout.txt", out);
}
