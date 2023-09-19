#include "type_traits.h"

class MyClass
{
public:
	MyClass() = default;
	static void foo(const int& a, const std::string& s)
	{
		return;
	}
};

int main()
{
	MyClass::foo(1, "");
	std::cout << (std::is_same<traits::RemoveReference<int&>, int>::type::value) << std::endl;
}