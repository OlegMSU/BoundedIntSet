#include <chrono>
#include <iostream>
#include <memory>
#include "Source.h"
#include <unordered_set>


int main()
{
	srand(time(0));

	const unsigned amount = 10000;
	const unsigned cycles = 10000;
	const unsigned k = 20;



	BoundedIntSet<unsigned, unsigned>* arr[amount];
	std::unordered_set<unsigned>* arr2[amount];
	for (auto i = 0; i < amount; i++)
	{
		unsigned r = rand();
		arr[i] = new BoundedIntSet<unsigned, unsigned>(k, r);
		arr2[i] = new std::unordered_set<unsigned>{ r };
	}


	auto sum1 = 0;
	auto sum2 = 0;
	auto len = 0;
	for (auto i = 0; i < cycles; i++)
	{
		unsigned i1 = rand() % amount;
		unsigned i2 = rand() % amount;
		auto start_time = std::chrono::steady_clock::now();
		if (arr[i1]->UnionCheck(arr[i2])) {
			auto end_time = std::chrono::steady_clock::now();
			auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
			std::cout << elapsed_ns.count() << " ns ";
			start_time = std::chrono::steady_clock::now();
			arr[i1]->Unite(arr[i2]);
			end_time = std::chrono::steady_clock::now();
			elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
			sum1 += elapsed_ns.count();
			std::cout << elapsed_ns.count() << " ns ";
			start_time = std::chrono::steady_clock::now();
			arr2[i1]->merge(*arr2[i2]);
			end_time = std::chrono::steady_clock::now();
			elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
			sum2 += elapsed_ns.count();
			std::cout << elapsed_ns.count() << " ns \n";
		}
	}




	std::cout << sum1 << " ns\n";
	std::cout << sum2 << " ns\n";

	return 0;
}
