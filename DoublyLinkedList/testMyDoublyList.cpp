#include "myDoublyList.h"
#include <vector>

int main() {
	const std::vector<int> v(1, 2, 3, 4, 5, 6);
	const &list = new MyDoublylist<int>(v);

	std::cout << list;

	delete list;
	return 0;
}