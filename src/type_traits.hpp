#pragma once

#include <type_traits>

namespace traits {

template <typename T>
struct remove_reference
{
    using type = T;
};

template <typename T>
struct remove_reference<T&>
{
    using type = T;
};

template <typename T>
struct remove_reference<T&&>
{
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct remove_pointer
{
    using type = T;
};

template <typename T>
struct remove_pointer<T*>
{
    using type = T;
};

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template <typename T>
struct remove_const
{
    using type = T;
};

template <typename T>
struct remove_const<const T>
{
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_volatile
{
    using type = T;
};

template <typename T>
struct remove_volatile<volatile T>
{
    using type = T;
};

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
struct remove_cv
{
    using type = typename remove_const<typename remove_volatile<T>::type>::type;
};

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename T, typename U>
struct is_same: std::false_type
{};

template <typename T>
struct is_same<T, T>: std::true_type
{};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

template <typename T>
struct is_void: is_same<void, typename remove_cv<T>::type>
{};

template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

template <typename T>
struct is_nullptr: is_same<std::nullptr_t, typename remove_cv<T>::type>
{};

template <typename T>
inline constexpr bool is_nullptr_v = is_nullptr<T>::value;

template <typename T>
struct is_pointer: std::false_type
{};

template <typename T>
struct is_pointer<T*>: std::true_type
{};

template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

template <typename T>
struct is_reference: std::false_type
{};

template <typename T>
struct is_reference<T&>: std::true_type
{};

template <typename T>
struct is_reference<T&&>: std::true_type
{};

template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

template <typename T>
struct is_const: std::false_type
{};

template <typename T>
struct is_const<const T>: std::true_type
{};

template <typename T>
inline constexpr bool is_const_v = is_const<T>::value;

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
