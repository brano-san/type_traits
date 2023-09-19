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

	class MyClass
	{
	public:
		MyClass() = default;
		static void foo(const int& a, const std::string& s) {	}
	};

}

namespace traits
{
	//Has* functions
	//HasMyFoo (foo(int const&, string))
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

	//is void
	template<typename T>
	struct IsVoid : IsSame<void, T>
	{	};

	//is nullptr
	template<typename T>
	struct IsNullptr : IsSame<std::nullptr_t, T>
	{	};

	//Remove* functions
	//utility function
	template<typename T>
	struct Remove
	{
		using type = T;
	};

	//RemoveReference
	template<typename T>
	struct RemoveReference : Remove<T>
	{};

	template<typename T>
	struct RemoveReference<T&> : Remove<T>
	{};

	template<typename T>
	struct RemoveReference<T&&> : Remove<T>
	{};

	//RemovePointer
	template<typename T>
	struct RemovePointer : Remove<T>
	{};

	template<typename T>
	struct RemovePointer<T*> : Remove<T>
	{};

	//RemoveConst
	template<typename T>
	struct RemoveConst : Remove<T>
	{};

	template<typename T>
	struct RemoveConst<const T> : Remove<T>
	{};

	//RemoveVolatile
	template<typename T>
	struct RemoveVolatile : Remove<T>
	{};

	template<typename T>
	struct RemoveVolatile<volatile T> : Remove<T>
	{};

	//RemoveCV
	template<typename T>
	struct RemoveCv : Remove<T>
	{};

	template<typename T>
	struct RemoveCv<const T> : Remove<T>
	{};

	template<typename T>
	struct RemoveCv<volatile T> : Remove<T>
	{};

	template<typename T>
	struct RemoveCv<const volatile T> : Remove<T>
	{};
}