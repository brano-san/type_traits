#pragma once

#include <tuple>
#include <functional>
#include <type_traits>

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

template <bool V>
using bool_constant = integral_constant<bool, V>;

using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

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

template <typename T, std::size_t N>
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

template <typename T, std::size_t N>
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

template <typename T, typename U>
struct common_type
{
    using type = decltype(declval<bool&>() ? declval<T>() : declval<U>());
};

template <typename T, typename U>
using common_type_t = typename common_type<T, U>::type;

template <typename T>
struct negation: bool_constant<!static_cast<bool>(T::value)>
{};

template <typename T>
inline constexpr bool negation_v = negation<T>::value;

template <typename T, typename U, typename = void>
struct is_addible: false_type
{};

template <typename T, typename U>
struct is_addible<T, U, std::void_t<decltype(declval<T>() + declval<U>())>>: true_type
{};

template <typename T, typename U = T>
inline constexpr bool is_addible_v = is_addible<T, U>::value;

template <typename T, typename U, typename = void>
struct is_equality_comparable: false_type
{};

template <typename T, typename U>
struct is_equality_comparable<T, U, void_t<decltype(declval<T>() == declval<U>())>>: true_type
{};

template <typename T, typename U = T>
inline constexpr bool is_equality_comparable_v = is_equality_comparable<T, U>::value;

template <typename T, typename U, typename = void>
struct is_assignable: false_type
{};

template <typename T, typename U>
struct is_assignable<T, U, void_t<decltype(declval<T&>() = declval<U&>())>>: true_type
{};

template <typename T, typename U>
inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

template <typename T, typename = void>
struct is_copy_assignable: false_type
{};

template <typename T>
struct is_copy_assignable<T, void_t<decltype(declval<T&>() = declval<const T&>())>>: true_type
{};

template <typename T>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

template <typename... Args>
struct conjunction: std::true_type
{};

template <typename T>
struct conjunction<T>: T
{};

template <typename Head, typename... Tail>
struct conjunction<Head, Tail...>: std::conditional_t<static_cast<bool>(Head::value), conjunction<Tail...>, Head>
{};

template <typename... Args>
inline constexpr bool conjunction_v = conjunction<Args...>::value;

template <typename... Args>
struct disjunction: false_type
{};

template <typename T>
struct disjunction<T>: T
{};

template <typename Head, typename... Tail>
struct disjunction<Head, Tail...>: conditional_t<static_cast<bool>(Head::value), Head, disjunction<Tail...>>
{};

template <typename... Args>
inline constexpr bool disjunction_v = disjunction<Args...>::value;

template <typename... Tail>
struct is_all_same: std::true_type
{};

template <typename First, typename... Tail>
struct is_all_same<First, Tail...>: bool_constant<(is_same_v<First, Tail> && ...)>
{};

template <typename... Args>
inline constexpr bool is_all_same_v = is_all_same<Args...>::value;

template <typename... Tail>
struct is_all_same_decay: std::true_type
{};

template <typename First, typename... Tail>
struct is_all_same_decay<First, Tail...>: bool_constant<(is_same_v<decay_t<First>, decay_t<Tail>> && ...)>
{};

template <typename... Args>
inline constexpr bool is_all_same_decay_v = is_all_same_decay<Args...>::value;

template <typename Base, typename Derived, typename = void>
struct is_base_of_impl: false_type
{};

template <typename Base, typename Derived>
struct is_base_of_impl<Base, Derived, void_t<decltype(static_cast<const Base*>(declval<const Derived*>()))>>: true_type
{};

template <typename Base, typename Derived>
struct is_base_of: bool_constant<(std::is_class_v<Base> && std::is_class_v<Derived> && is_base_of_impl<Base, Derived>::value) ||
                                 (std::is_class_v<Base> && is_same_v<Base, Derived>)>
{};

template <typename Base, typename Derived>
inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

template <typename... Args>
struct args_count_helper;

template <>
struct args_count_helper<>
{
    static constexpr std::size_t value = 0;
};

template <typename Head, typename... Tail>
struct args_count_helper<Head, Tail...>
{
    static constexpr std::size_t value = 1 + args_count_helper<Tail...>::value;
};

template <typename... Args>
inline constexpr std::size_t args_count_v = args_count_helper<Args...>::value;

template <std::size_t N, typename... Args>
struct argument
{};

template <typename Head, typename... Tail>
struct argument<0, Head, Tail...>
{
    using type = Head;
};

template <std::size_t N, typename Head, typename... Tail>
struct argument<N, Head, Tail...>: argument<N - 1, Tail...>
{};

template <std::size_t N, typename... Args>
using argument_t = typename argument<N, Args...>::type;

template <typename R, typename... Args>
struct function_traits_info
{
    using plain_signature = R(Args...);

    using args_tuple = std::tuple<Args...>;

    using return_type                  = R;
    static constexpr std::size_t arity = sizeof...(Args);

    template <std::size_t N>
    using arg_t = argument_t<N, Args...>;
};

template <typename R, typename... Args>
struct function_traits
{};

template <typename R, typename... Args>
struct function_traits<R(Args...)>: function_traits_info<R, Args...>
{
    static constexpr bool is_const    = false;
    static constexpr bool is_noexcept = false;
};

template <typename R, typename... Args>
struct function_traits<R(Args...) noexcept>: function_traits_info<R, Args...>
{
    static constexpr bool is_const    = false;
    static constexpr bool is_noexcept = true;
};

template <typename R, typename... Args>
struct function_traits<R (*)(Args...)>: function_traits<R(Args...)>
{};

template <typename R, typename... Args>
struct function_traits<R (*)(Args...) noexcept>: function_traits<R(Args...) noexcept>
{};

template <typename R, typename T, typename... Args>
struct function_traits<R (T::*)(Args...)>: function_traits_info<R, Args...>
{
    using class_type                  = T;
    static constexpr bool is_const    = false;
    static constexpr bool is_noexcept = false;
};

template <typename R, typename T, typename... Args>
struct function_traits<R (T::*)(Args...) const>: function_traits_info<R, Args...>
{
    using class_type                  = T;
    static constexpr bool is_const    = true;
    static constexpr bool is_noexcept = false;
};

template <typename R, typename T, typename... Args>
struct function_traits<R (T::*)(Args...) noexcept>: function_traits_info<R, Args...>
{
    using class_type                  = T;
    static constexpr bool is_const    = false;
    static constexpr bool is_noexcept = true;
};

template <typename R, typename T, typename... Args>
struct function_traits<R (T::*)(Args...) const noexcept>: function_traits_info<R, Args...>
{
    using class_type                  = T;
    static constexpr bool is_const    = true;
    static constexpr bool is_noexcept = true;
};

template <typename AlwaysVoid, typename F, typename... Args>
struct is_invokable: false_type
{};

template <typename F, typename... Args>
struct is_invokable<std::void_t<decltype(std::invoke(declval<F>(), declval<Args>()...))>, F, Args...>: true_type
{};

template <typename F, typename... Args>
inline constexpr bool is_invokable_v = is_invokable<void, F, Args...>::value;

struct nonesuch
{
    ~nonesuch() = delete;
};

template <class Default, class AlwaysVoid, template <typename...> class F, class... Args>
struct detector
{
    using value_t = false_type;
    using type    = Default;
};

template <class Default, template <typename...> class F, class... Args>
struct detector<Default, std::void_t<F<Args...>>, F, Args...>: true_type
{
    using value_t = true_type;
    using type    = F<Args...>;
};

template <template <typename...> class F, class... Args>
using is_detected = typename detector<nonesuch, void, F, Args...>::value_t;

template <template <typename...> class Op, typename... Args>
inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;

// - Анализ типов с переменным количеством шаблонных параметров (допустим std::tuple):
//     - lenght<T> - возвращает количество шаблонных параметров
//     - type_at<N, T> - возвращает N-ый тип из списка шаблонных параметров
//     - contains<U, T> - содержит ли T в списке шаблонных параметров U
//     - push_back<T, U> - добавляет тип в конец списка шаблонных параметров

template <template <typename... Args> class T>
struct tuple_analysis
{};

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
