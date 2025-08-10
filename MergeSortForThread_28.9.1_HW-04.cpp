#include "mergesortarray.h"

#include <iostream>

int main()
{
	std::cout << "Threading sorting.\n";
	std::cout << "Hello!\nWe are testing the threading sorting \n"
		<< "of array elements with a different number of threads.\n";

	Array array;
	array.initArray();

	return 0;
}
