// Melody.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>
#include <fstream>

#define __long int64_t

int main()
{
	std::ifstream in("melodyin.txt");
	std::ofstream out("melodyout.txt");

	__long num, max;
	in >> num >> max;

	std::vector<__long> notes;
	notes.reserve(num);

	__long temp;
	for (__long i = 0; i < num; i++)
	{
		in >> temp;
		std::cout << temp << '\n';
		notes.push_back(temp);
	}

	__long minChanges = 0;
	for (__long i = 0; i < num / 3; i++)
	{
		/*__long iterChanges = 0;
		for (__long ii = 0; ii < num / 3; ii++)
		{
			if (notes[ii * 3] != notes[i * 3])
				iterChanges++;
			if (notes[ii * 3 + 1] != notes[i * 3 + 1])
				iterChanges++;
			if (notes[ii * 3 + 2] != notes[i * 3 + 2])
				iterChanges++;
		}
		if (iterChanges < minChanges)
			minChanges = iterChanges;*/

		if (notes[i * 3] != 1)
			minChanges++;
		if (notes[i * 3 + 1] != 2)
			minChanges++;
		if (notes[i * 3 + 2] != 3)
			minChanges++;
	}

	std::cout << (minChanges == INT64_MAX ? 0 : minChanges) << '\n';
	out << (minChanges == INT64_MAX ? 0 : minChanges);
}