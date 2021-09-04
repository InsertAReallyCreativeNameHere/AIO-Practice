// AIO 2021.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#define __long int64_t

struct vec2
{
	__long x, y;
};

int main()
{
	std::ifstream in("robotin.txt");
	std::ofstream out("robotout.txt");

	size_t len;
	in >> len;

	std::string str(len, 0);

	while (in.read(&str[0], 1), str[0] != '\n');
	in.read(&str[0], len);

	std::cout << len << '\n' << str << '\n';

	vec2 moves { 0, 0 };
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		switch (*it)
		{
		case 'N':
			++moves.y;
			break;
		case 'E':
			++moves.x;
			break;
		case 'S':
			--moves.y;
			break;
		case 'W':
			--moves.x;
			break;
		}
	}

	out << std::abs(moves.x) + std::abs(moves.y);
	std::cout << std::abs(moves.x) + std::abs(moves.y) << '\n';
}
