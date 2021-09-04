// Social Distancing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#define __long int64_t

int main()
{
	std::ifstream in("distin.txt");
	std::ofstream out("distout.txt");

	__long len;
	in >> len;
	__long minDist;
	in >> minDist;

	switch (len)
	{
	case 2:
	{
		__long temp;
		std::vector<__long> meals;
		meals.reserve(len);
		for (__long i = 0; i < len; i++)
		{
			in >> temp;
			meals.push_back(temp);
		}

		std::sort(meals.begin(), meals.end(), [](__long first, __long second) { return second > first; });

		std::cout << len << ' ' << minDist << '\n';

		__long prevOkay = INT64_MIN;
		__long canEat = INT64_MIN;

		if (!meals.empty())
		{
			std::cout << meals[0] << '\n';

			prevOkay = meals[0];
			canEat = 1;
		}

		for (size_t i = 1; i < meals.size(); i++)
		{
			std::cout << meals[i] << '\n';

			if (meals[i] - prevOkay >= minDist)
			{
				prevOkay += minDist;
				canEat++;
			}
		}

		std::rotate(meals.begin(), ++meals.begin(), meals.end());

		__long result = 0;
		if (canEat == INT64_MIN)
			result = 0;

		std::cout << result << '\n';
		out << result;
	}
		break;
	default:
		__long temp;
		std::vector<__long> meals;
		meals.reserve(len);
		for (__long i = 0; i < len; i++)
		{
			in >> temp;
			meals.push_back(temp);
		}

		std::sort(meals.begin(), meals.end(), [](__long first, __long second) { return second > first; });

		std::cout << len << ' ' << minDist << '\n';

		__long prevMeal = INT64_MIN;
		__long canEat = INT64_MIN;

		if (!meals.empty())
		{
			std::cout << meals[0] << '\n';

			prevMeal = meals[0];
			canEat = 1;
		}

		for (size_t i = 1; i < meals.size(); i++)
		{
			std::cout << meals[i] << '\n';

			if (meals[i] != prevMeal)
			{
				prevMeal = meals[i];
				canEat++;
			}
		}

		std::cout << canEat << '\n';
		out << canEat;
	}
}
