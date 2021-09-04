// Laser Cutter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#define __long int64_t

struct tilemap
{
	__long dim;
	std::vector<bool> filled;

	bool at(__long row, __long col)
	{
		return this->filled[col * dim + row];
	}
	void set(__long row, __long col, bool val)
	{
		this->filled[col * dim + row] = val;
	}
};

int main()
{
	std::ifstream in("laserin.txt");
	std::ofstream out("laserout.txt");

	__long dim;
	std::string a, b;

	in >> dim;

	a.reserve(dim * 2);
	b.reserve(dim * 2);

	in.read(&a[0], 1);
	in >> a;
	in.read(&b[0], 1);
	in >> b;

	tilemap tm{ dim };
	tm.filled.resize(dim * dim, true);

	size_t index = 0;
	__long rowSeeker = 0;
	for (__long columnSeeker = 0; columnSeeker < dim;)
	{
		switch (b[index])
		{
		case 'D':
			rowSeeker++;
			break;
		case 'R':
			for (__long j = 0; j < rowSeeker; j++)
			{
				tm.set(j, columnSeeker, false);
			}
			columnSeeker++;
			break;
		}
		index++;
	}

	for (size_t i = 0; i < dim; i++)
	{
		for (size_t j = 0; j < dim; j++)
		{
			std::cout << tm.at(i, j) << ' ';
		}
		std::cout << '\n';
	}

	size_t maxSize = 0;
	for (size_t test = 0; test < dim; test++)
	{
		for (__long row = dim - test; row < dim; row++)
		{
			for (__long col = 0; col < test; col++)
			{
				if (tm.at(row, col) == false)
					goto BreakAll;
			}
		}
		maxSize++;
	}
BreakAll:;

	std::cout << maxSize << '\n';
	out << maxSize;
}
