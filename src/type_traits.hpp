#pragma once

#include <type_traits>

/* TODO List:
 - is_signed
 - is_all_same (by conjunction)
 - common_type (by ?:)
 - is_base_of

 - is_addible<T, U>
 - is_equality_comparable<T, U>  - T == U
 - is_copy_assignable<T> - T t1, t2; t1 = t2;

 - conjunction - Логическое И
 - disjunction - Логическое ИЛИ
 - negation - Логическое Не

 - function_traits:   function_traits<F>::return_type  +  function_traits<F>::argument<0>::type
   - F(Args...)
   - F(*)(Args...)
   - F(T::*)(Args...)
   - F(T::*)(Args...) const

 - is_invokable: (By args)
   - is_invocable_v<void(int, double), int, float> - true
   - is_invocable_v<void(int), std::string>        - false

 - is_detected

 - Анализ типов с переменным количеством шаблонных параметров (допустим std::tuple):
   - lenght<T> - возвращает количество шаблонных параметров
   - type_at<N, T> - возвращает N-ый тип из списка шаблонных параметров
   - contains<U, T> - содержит ли T в списке шаблонных параметров U
   - push_back<T, U> - добавляет тип в конец списка шаблонных параметров
*/

namespace traits {

template <typename... Args>
using void_t = void;

template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;

    constexpr operator T() const noexcept
    {
        return value;
    }

    constexpr T operator()() const noexcept
    {
        return value;
    }
};

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

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
struct add_rvalue_reference
{
    using type = T&&;
};

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <typename T>
add_rvalue_reference_t<T> declval() noexcept
{
    static_assert(false, "Cannot call to declval function");
}

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
    using type = remove_const_t<remove_volatile_t<T>>;
};

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename T>
struct remove_extent
{
    using type = T;
};

template <typename T>
struct remove_extent<T[]>
{
    using type = T;
};

template <typename T, size_t N>
struct remove_extent<T[N]>
{
    using type = T;
};

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

template <typename T>
struct add_pointer
{
    using type = traits::remove_reference_t<T>*;
};

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

template <typename T, typename U>
struct is_same: false_type
{};

template <typename T>
struct is_same<T, T>: true_type
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
struct is_array: false_type
{};

template <typename T>
struct is_array<T[]>: true_type
{};

template <typename T, size_t N>
struct is_array<T[N]>: true_type
{};

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

template <typename T>
struct signed_base
{
private:
    using value_type = remove_cv_t<T>;

public:
    static constexpr bool is_signed   = static_cast<value_type>(-1) < static_cast<value_type>(1);
    static constexpr bool is_unsigned = !is_signed;
};

template <typename T>
struct is_signed: integral_constant<bool, signed_base<T>::is_signed>
{};

template <typename T>
inline constexpr bool is_signed_v = is_signed<T>::value;

template <typename T>
struct is_unsigned: integral_constant<bool, signed_base<T>::is_unsigned>
{};

template <typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

template <typename T>
struct is_pointer_helper: false_type
{};

template <typename T>
struct is_pointer_helper<T*>: true_type
{};

template <typename T>
struct is_pointer: is_pointer_helper<remove_cv_t<T>>
{};

template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

template <typename T>
struct is_reference: false_type
{};

template <typename T>
struct is_reference<T&>: true_type
{};

template <typename T>
struct is_reference<T&&>: true_type
{};

template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

template <typename T>
struct is_const: false_type
{};

template <typename T>
struct is_const<const T>: true_type
{};

template <typename T>
inline constexpr bool is_const_v = is_const<T>::value;

template <typename T, typename = void>
struct has_iterator: false_type
{};

template <typename T>
struct has_iterator<T, void_t<typename T::iterator>>: true_type
{};

template <typename T>
inline constexpr bool has_iterator_v = has_iterator<T>::value;

template <typename T, typename = void>
struct is_container: false_type
{};

template <typename T>
struct is_container<T, void_t<decltype(declval<T&>().begin()), decltype(declval<T&>().end())>>: true_type
{};

template <typename T>
inline constexpr bool is_container_v = is_container<T>::value;

template <bool Cond, typename T, typename U>
struct conditional
{
    using type = T;
};

template <typename T, typename U>
struct conditional<false, T, U>
{
    using type = U;
};

template <bool Cond, typename T, typename U>
using conditional_t = typename conditional<Cond, T, U>::type;

template <bool Cond, typename T = void>
struct enable_if;

template <typename T>
struct enable_if<true, T>
{
    using type = T;
};

template <bool Cond, typename T = void>
using enable_if_t = typename enable_if<Cond, T>::type;

template <typename T>
class decay
{
    using U = remove_reference_t<T>;

public:
    using type = conditional_t<is_array_v<U>, add_pointer_t<remove_extent_t<U>>,
        conditional_t<std::is_function_v<U>, add_pointer_t<U>, remove_cv_t<U>>>;
};

template <typename T>
using decay_t = typename decay<T>::type;

namespace imagine {
template <typename T, typename = void>
struct has_member_foo: false_type
{};

template <typename T>
struct has_member_foo<T, void_t<decltype(T::foo)>>: true_type
{};

template <typename T>
inline constexpr bool has_member_foo_v = has_member_foo<T>::value;
}  // namespace imagine

}  // namespace traits
