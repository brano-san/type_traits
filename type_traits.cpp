#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <set>

namespace traits
{
	template<typename T>
	class HasBeginEnd
	{
	private:
	    template<class U>
	    static constexpr auto cheker(U* u) -> decltype((*u).begin(), (*u).end(), bool()) { return true; }

	    template<class>
	    static constexpr auto cheker(...) { return false; }
		
	public:
	    static bool constexpr value = cheker<T>(nullptr);
	};

	template<typename T>
	class HasConstBeginEnd
	{
	private:
	    template<class U>
	    static constexpr auto constCheker(U* u) -> decltype((*u).cbegin(), (*u).cend(), bool()) { return true; }

	    template<class>
	    static constexpr auto constCheker(...) { return false; }

	public:
	    static bool constexpr value = constCheker<T>(nullptr);
	};

	template<typename T, typename U>
	struct IsSame
	{
	    static bool constexpr value = false;
	};

	template<typename T>
	struct IsSame<T, T>
	{
	    static bool constexpr value = true;
	};

	template<typename T>
	struct IsVoid : IsSame<void, T>
	{	};

	template<typename T>
	struct RemoveReference
	{
		using type = T;
	};

	template<typename T>
	struct RemoveReference<T&>
	{
		using type = T;
	};

	template<typename T>
	struct RemoveReference<T&&>
	{
		using type = T;
	};
}

int main()
{
	std::cout << traits::IsSame<traits::RemoveReference<int>::type, int>::value << std::endl;
	std::cout << traits::IsSame<traits::RemoveReference<int&>::type, int>::value << std::endl;
	std::cout << traits::IsSame<traits::RemoveReference<int&&>::type, int>::value << std::endl;
}
