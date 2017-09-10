#include <iostream>

#include "pewpew.hpp"

/*
NOTE:
- When adding floats, it only can push max 8 floats at stack
  so when adding >8 floats, the parser generates an
  unbalanced tree, so that the compiler has to push 
  >8 floats at stack, which causes undefined behaviour >.<
  SOLUTION: balance tree generation
*/

static const std::string source = R"(
	c = lambda(a, b) -> () {
		if true 
		{
		}
	}
)";

int main()
{
	pewpew pew(source);

	//std::cout << pew.get<float>("a") << '\n';
	//std::cout << pew.get<float>("b") << '\n';

	auto fn = get<float*, float*, float*, float*, float*, float*, float*, float*, float*, float*>::var<int, int>::call(pew, "c");

	//auto fn = pew.get_func<float*, float*, float*, float*>("c");
	float x = 0, y = 0, z = 0, w = 0, v = 0, x1 = 0, y1 = 0, z1 = 0, w1 = 0, v1 = 0;
	fn(11, 22, &x, &y, &z, &w, &v, &x1, &y1, &z1, &w1, &v1);
	std::cout << x << '\n';
	std::cout << y << '\n';
	std::cout << z << '\n';
	std::cout << w << '\n';
	std::cout << v << '\n';
	std::cout << x1 << '\n';
	std::cout << y1 << '\n';
	std::cout << z1 << '\n';
	std::cout << w1 << '\n';
	std::cout << v1 << '\n';

	return 0;
}