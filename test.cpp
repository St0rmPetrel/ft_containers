#include <vector>
#include <iostream>
//#include "vector.hpp"

int main() {
	std::vector<int>	a = {1, 2};
	std::vector<int>::iterator<int> it = a.begin();
	int b; *it = b;

	std::cout << b;
}
