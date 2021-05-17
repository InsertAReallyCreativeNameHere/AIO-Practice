#include <stdinc.h>
#include <defines.h>

using namespace Marble;

int main()
{
    FileInput("sitin.txt");

    uint64 ticketsBought;
    uint16 rows;
    uint16 seatsPerRow;
    {
        auto split1 = input.split("\n");
        ticketsBought = (++split1.begin())->toIntegralType<uint64>();
        auto split2 = split1.begin()->split(" ");
        rows = split2.begin()->toIntegralType<uint16>();
        seatsPerRow = (++split2.begin())->toIntegralType<uint16>();
    }

    uint64 totalSeats = (uint64)rows * seatsPerRow;
    string out = string::format("{} {}", totalSeats > ticketsBought ? ticketsBought : totalSeats, ticketsBought > totalSeats ? ticketsBought - totalSeats : 0);

    FileOutput("sitout.txt", out);
}
