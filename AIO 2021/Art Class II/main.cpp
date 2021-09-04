// Art Class II.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#define __long int64_t

int main()
{
	std::ifstream in("artin.txt");
	std::ofstream out("artout.txt");

	__long len;
	in >> len;

	std::cout << len << '\n';

	__long minX = 0, minY = 0, maxX = 0, maxY = 0;

	__long x, y;
	in >> x;
	in >> y;
	minX = x;
	maxX = x;
	minY = y;
	maxY = y;

	std::cout << x << ' ' << y << '\n';

	for (__long i = 1; i < len; ++i)
	{
		__long x, y;
		in >> x;
		in >> y;

		std::cout << x << ' ' << y << '\n';

		if (x > maxX)
			maxX = x;
		if (x < minX)
			minX = x;
		if (y > maxY)
			maxY = y;
		if (y < minY)
			minY = y;
	}

	__long area;
	area = (maxX - minX) * (maxY - minY);

	out << area;
	std::cout << area << '\n';
}
