#include <iostream>
#include <scanner.hpp>
#include <string>

int main() {
	scanner::Scanner s1{};
	std::string bytes = "1+233-(1-100)+2";
	auto t = s1.scan(bytes);
	for (auto &token : *t) {
		std::cout << token.toString() << std::endl;
	}
	delete t;
	std::cout << std::endl;
	return 0;
}
