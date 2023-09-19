#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <set>
#include <utility>

namespace test_classes
{
	class PureClass
	{
	public:

	};

	class BeginEnd
	{
	public:
		void begin() {    }
		void end() {	}
	};

	class CBeginEnd
	{
	public:
		void cbegin() {   }
		void cend() {	}
	};
}

namespace traits
{
	//Has* functions
	template<typename T>
	class HasMyFoo
	{
	private:
		template<class U>
		static constexpr auto cheker(U* u) -> decltype((*u).foo(std::declval<int const&>(), ""), bool()) { return true; }

		template<class>
		static constexpr auto cheker(...) { return false; }

	public:
		static bool constexpr value = cheker<T>(nullptr);
	};

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

	//Is* functions
	//IsSame
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

	//IsVoid
	template<typename T>
	struct IsVoid : IsSame<void, T>
	{	};

	//Remove* functions
	//RemoveReference
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

	//RemovePointer
	template<typename T>
	struct RemovePointer
	{
		using type = T;
	};

	template<typename T>
	struct RemovePointer<T*>
	{
		using type = T;
	};

	//RemoveConst
	template<typename T>
	struct RemoveConst
	{
		using type = T;
	};

	template<typename T>
	struct RemoveConst<const T>
	{
		using type = T;
	};
}
