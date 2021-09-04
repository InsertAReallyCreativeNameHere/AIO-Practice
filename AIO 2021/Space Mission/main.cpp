// Space Mission.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>
#include <fstream>

#define __long int32_t

int main()
{
	std::ifstream in("spacein.txt");
	std::ofstream out("spaceout.txt");

	__long days, fuel;
	in >> days >> fuel;

	std::pair<__long, __long> bestPair{ 2, 0 };
	std::vector<__long> costsPerDay;
	costsPerDay.reserve(days);
	
	__long temp;
	for (__long i = 0; i < days; i++)
	{
		in >> temp;
		costsPerDay.push_back(temp);
	}

	for (__long i = 0; i < days; i++)
	{
		for (__long j = 0; j < days; j++)
		{
			if (i != j && costsPerDay[i] + costsPerDay[j] <= fuel && bestPair.second - bestPair.first < j - i)
			{
				bestPair = { i,j };
			}
		}
	}

	std::cout << bestPair.second - bestPair.first + 1 << '\n';
	out << bestPair.second - bestPair.first + 1;
}
