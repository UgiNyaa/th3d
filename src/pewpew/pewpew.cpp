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
	c = lambda(a) -> () {
		return a, 20, 30, 40, 50
	}
)";

int main()
{
	pewpew pew(source);

	//std::cout << pew.get<float>("a") << '\n';
	//std::cout << pew.get<float>("b") << '\n';

	auto fn = get<float*, float*, float*, float*, float*>::var<int>::call(pew, "c");

	//auto fn = pew.get_func<float*, float*, float*, float*>("c");
	float x = 0, y = 0, z = 0, w = 0, g = 0;
	fn(11, &x, &y, &z, &w, &g);
	std::cout << x << ' ' << y << ' ' << z << ' ' << w << ' ' << g << '\n';

	return 0;
}