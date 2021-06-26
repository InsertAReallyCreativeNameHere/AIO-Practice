#include <stdinc.h>
#include <defines.h>

using namespace Marble;

struct Date final
{
    size_t year, month, date;
};

int main()
{
    FileInput("input.txt");

    std::vector<string> split = input.split("\n");
    size_t numDates = split[0].toIntegralType<size_t>();
    split.erase(split.begin());

    string output;
    for (auto it = split.begin(); it != split.end(); ++it)
    {
        string line;
        line += *it;
        line += " is a ";

        std::vector<string> ymd = it->split("-");
        string year = ymd[0];
        string _Y(2);
        _Y.operator[](0) = year.operator[](2);
        _Y.operator[](1) = year.operator[](3);
        string _C(2);
        _C.operator[](0) = year.operator[](0);
        _C.operator[](1) = year.operator[](1);
        __int Y = _Y.toIntegralType<__int>();
        __int C = _C.toIntegralType<__int>();
        __int m = ymd[1].toIntegralType<__int>();
        __int d = ymd[2].toIntegralType<__int>();
        if (m <= 0 || m > 12)
        {
            line += "not valid date (stop trying to break my code!)";
            goto End;
        }
        if (d <= 0)
        {
            line += "not valid date (stop trying to break my code!)";
            goto End;
        }
        switch (m)
        {
        case 1:
            if (d > 31)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 2:
            if ((Y + C * 100) % 4 == 0)
            {
                if (d > 29)
                {
                    line += "not valid date (stop trying to break my code!)";
                    goto End;
                }
            }
            else
            {
                if (d > 28)
                {
                    line += "not valid date (stop trying to break my code!)";
                    goto End;
                }
            }
            break;
        case 3:
            if (d > 31)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 4:
            if (d > 30)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 5:
            if (d > 31)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 6:
            if (d > 30)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 7:
            if (d > 31)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 8:
            if (d > 31)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 9:
            if (d > 30)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 10:
            if (d > 31)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 11:
            if (d > 30)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        case 12:
            if (d > 31)
            {
                line += "not valid date (stop trying to break my code!)";
                goto End;
            }
            break;
        }

        {
            m -= 2;
            if (m <= 0)
            {
                m += 12;
                Y -= 1;
            }
            if (Y <= 0)
            {
                Y = 99;
                C -= 1;
            }

            __int w = (d + __int(2.6*double(m) - 0.2) + Y + __int(double(Y)/4.0) + 5*C + __int(double(C)/4.0)) % 7;
            switch (w)
            {
            case 0:
                line += "Sunday";
                break;
            case 1:
                line += "Monday";
                break;
            case 2:
                line += "Tuesday";
                break;
            case 3:
                line += "Wednesday";
                break;
            case 4:
                line += "Thursday";
                break;
            case 5:
                line += "Friday";
                break;
            case 6:
                line += "Saturday";
                break;
            }
        }

        End:
        output += line;
        output += "\n";
    }

    FileOutput("output.txt", output);
}
