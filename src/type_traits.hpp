#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <set>
#include <utility>

namespace traits {

template <typename T>
struct Remove
{
    using type = T;
};

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

template <typename T>
struct RemovePointer: Remove<T>
{};

template <typename T>
struct RemovePointer<T*>: Remove<T>
{};

template <typename T>
using RemovePointerT = typename RemovePointer<T>::type;

template <typename T>
struct RemoveConst: Remove<T>
{};

template <typename T>
struct RemoveConst<const T>: Remove<T>
{};

template <typename T>
using RemoveConstT = typename RemoveConst<T>::type;

template <typename T>
struct RemoveVolatile: Remove<T>
{};

template <typename T>
struct RemoveVolatile<volatile T>: Remove<T>
{};

template <typename T>
using RemoveVolatileT = typename RemoveVolatile<T>::type;

template <typename T>
struct RemoveCv
{
    using type = typename RemoveConst<typename RemoveVolatile<T>::type>::type;
};

template <typename T>
using RemoveCvT = typename RemoveCv<T>::type;

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

template <typename T>
struct IsConst: std::false_type
{};

template <typename T>
struct IsConst<const T>: std::true_type
{};

template <typename T>
inline constexpr bool IsConstV = IsConst<T>::value;

namespace imagine {

template <typename T, typename = void>
struct has_member_foo: std::false_type
{};

template <typename T>
struct has_member_foo<T, std::void_t<decltype(T::foo)>>: std::true_type
{};

template <typename T>
inline constexpr bool has_member_foo_v = has_member_foo<T>::value;

}  // namespace imagine
}  // namespace traits
