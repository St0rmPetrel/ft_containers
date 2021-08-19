#include "vector.hpp"
#include <iostream>
#include <vector>

int main() {
	std::vector<int> a;
	ft::vector<int> b(a.max_size() + 1);

	std::cout << a.max_size() << std::endl;
	std::cout << b.max_size() << std::endl;
}
