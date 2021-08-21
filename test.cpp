#include "vector.hpp"
#include <iostream>
#include <vector>

int main() {
	ft::vector<int> a;

	a.resize(1, 100);
	ft::vector<int> b(a);
	for (ft::vector<int>::iterator it = b.begin(); it != b.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}
