#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <set>
#include <utility>

namespace test_classes {
class PureClass
{
public:
};

class BeginEnd
{
public:
    void begin() {}

    void end() {}
};

class CBeginEnd
{
public:
    void cbegin() {}

    void cend() {}
};

class MyClass
{
public:
    MyClass() = default;

    static void foo(const int& a, const std::string& s) {}
};

}  // namespace test_classes

namespace traits {
// Has* functions
// HasMyFoo (foo(int const&, string))
template <typename T>
class HasMyFoo
{
private:
    template <class U>
    static constexpr auto cheker(U* u) -> decltype((*u).foo(std::declval<const int&>(), ""), bool())
    {
        return true;
    }

    template <class>
    static constexpr auto cheker(...)
    {
        return false;
    }

public:
    static constexpr bool value = cheker<T>(nullptr);
};

template <typename T>
class HasBeginEnd
{
private:
    template <class U>
    static constexpr auto cheker(U* u) -> decltype((*u).begin(), (*u).end(), bool())
    {
        return true;
    }

    template <class>
    static constexpr auto cheker(...)
    {
        return false;
    }

public:
    static constexpr bool value = cheker<T>(nullptr);
};

template <typename T>
class HasConstBeginEnd
{
private:
    template <class U>
    static constexpr auto constCheker(U* u) -> decltype((*u).cbegin(), (*u).cend(), bool())
    {
        return true;
    }

    template <class>
    static constexpr auto constCheker(...)
    {
        return false;
    }

public:
    static constexpr bool value = constCheker<T>(nullptr);
};

// Remove* functions
// utility function
template <typename T>
struct Remove
{
    using type = T;
};

// RemoveReference
template <typename T>
struct RemoveReference: Remove<T>
{};

template <typename T>
struct RemoveReference<T&>: Remove<T>
{};

template <typename T>
struct RemoveReference<T&&>: Remove<T>
{};

template <typename T>
using RemoveReferenceT = typename RemoveReference<T>::type;

// RemovePointer
template <typename T>
struct RemovePointer: Remove<T>
{};

template <typename T>
struct RemovePointer<T*>: Remove<T>
{};

template <typename T>
using RemovePointerT = typename RemovePointer<T>::type;

// RemoveConst
template <typename T>
struct RemoveConst: Remove<T>
{};

template <typename T>
struct RemoveConst<const T>: Remove<T>
{};

template <typename T>
using RemoveConstT = typename RemoveConst<T>::type;

// RemoveVolatile
template <typename T>
struct RemoveVolatile: Remove<T>
{};

template <typename T>
struct RemoveVolatile<volatile T>: Remove<T>
{};

template <typename T>
using RemoveVolatileT = typename RemoveVolatile<T>::type;

// RemoveCV
template <typename T>
struct RemoveCv
{
    using type = typename RemoveConst<typename RemoveVolatile<T>::type>::type;
};

template <typename T>
using RemoveCvT = typename RemoveCv<T>::type;

// Is* functions
// IsSame
template <typename T, typename U>
struct IsSame: std::false_type
{};

template <typename T>
struct IsSame<T, T>: std::true_type
{};

template <typename T, typename U>
inline constexpr bool IsSameV = IsSame<T, U>::value;

template <typename T>
struct IsVoid: IsSame<void, typename RemoveCv<T>::type>
{};

template <typename T>
inline constexpr bool IsVoidV = IsVoid<T>::value;

template <typename T>
struct IsNullptr: IsSame<std::nullptr_t, typename RemoveCv<T>::type>
{};

template <typename T>
inline constexpr bool IsNullptrV = IsNullptr<T>::value;

template <typename T>
struct IsPointer: std::false_type
{};

template <typename T>
struct IsPointer<T*>: std::true_type
{};

template <typename T>
inline constexpr bool IsPointerV = IsPointer<T>::value;

template <typename T>
struct IsReference: std::false_type
{};

template <typename T>
struct IsReference<T&>: std::true_type
{};

template <typename T>
struct IsReference<T&&>: std::true_type
{};

template <typename T>
inline constexpr bool IsReferenceV = IsReference<T>::value;
}  // namespace traits
