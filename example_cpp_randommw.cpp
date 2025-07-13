// Example: simple generation of normally distributed random numbers
// in C++ (C++20)
//
// This program produces output identical to `example_randommw.c`.
//
// Compile with
// $ g++ example_cpp_randommw.cpp -o example_cpp.exe -std=c++20 -Wall -Wextra -Wpedantic 

#include <iostream>
#include <format>
#include "randommw.h"

int main(void) {
	unsigned int i;
	uint64_t zigseed = 10;
	double rval;
		
	RanInit("", zigseed, 0);
	
	for(i = 0; i < 20; i++)	{
		rval = DRanNormalZig();
		std::cout << std::format("{:10.6f}\n", rval);
	}
	
	return 0;
}
