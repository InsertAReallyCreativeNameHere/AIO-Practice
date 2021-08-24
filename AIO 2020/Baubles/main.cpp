#include <cstdint>
#include <iostream>
#include <fstream>

#define __long int64_t

int main()
{
	std::ifstream in("baublesin.txt");
	std::ofstream out("baublesout.txt");

	__long red;
	__long blue;
	__long spare;
	__long redOrdered;
	__long blueOrdered;

	in >> red;
	in >> blue;
	in >> spare;
	in >> redOrdered;
	in >> blueOrdered;

	std::cout << red << ' ' << blue << ' ' << spare << ' ' << redOrdered << ' ' << blueOrdered << '\n';

	auto redDiff = redOrdered - red;
	red += redDiff;
	spare -= redDiff;
	auto blueDiff = blueOrdered - blue;
	blue += blueDiff;
	spare -= blueDiff;

	if (redOrdered + blueOrdered < red + blue)
	{
		if (blueOrdered < redOrdered)
			spare -= blueOrdered;
		else spare -= redOrdered;
	}

	auto result = (spare < 0 ? 0 : spare + 1);

	std::cout << result << '\n';

	out << result;
}