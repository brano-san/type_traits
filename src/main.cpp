#include "type_traits.hpp"

#include <cstdint>
#include <list>
#include <string>
#include <vector>
#include <type_traits>  // Для проверок и сравнения со std::

using namespace traits;

namespace {

// ============================================================================
// 1. Вспомогательные типы и классы для тестов
// ============================================================================

struct A
{};

struct B: A
{};

struct C
{};

struct D: A,
          C
{};

struct PrivateBase
{};

struct PrivateDerived: private PrivateBase
{};

struct Base1
{
    int x;
};

struct Mid1: Base1
{};

struct Mid2: Base1
{};

struct Diamond: Mid1,
                Mid2
{};

struct Base
{};

struct Derived: Base
{};

class PureClass
{};

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

struct MyContainer
{
    using iterator = int*;
    void begin();
};

struct EmptyStruct
{
    void begin();
    void end();
};

struct ConvertibleToInt
{
    operator int() const
    {
        return 0;
    }
};

struct ConvertibleToDouble
{
    operator double() const
    {
        return 0.0;
    }
};

struct True
{
    static constexpr bool value = true;
};

struct False
{
    static constexpr bool value = false;
};

// ТИП-ЛОВУШКА: если к нему обратиться, компилятор должен упасть.
template <typename T>
struct ExplodingType
{
    static constexpr bool value = false;
    static_assert(false, "CONJUNCTION IS NOT SHORT-CIRCUITING!");
};

void func(int);  // Forward declaration for decay tests

// ============================================================================
// 2. Compile-time тесты (static_assert)
// ============================================================================

// --- Type Relationships (Отношения типов) ---
static_assert(is_base_of_v<A, B>, "Test 1 Failed: B should be base of A");
static_assert(is_base_of_v<A, A>, "Test 2 Failed: A should be base of itself");
static_assert(!is_base_of_v<A, C>, "Test 3 Failed: C is not base of A");
static_assert(is_base_of_v<A, D>, "Test 4 Failed: D inherits A");
static_assert(is_base_of_v<C, D>, "Test 4 Failed: D inherits C");
static_assert(!is_base_of_v<int, B>, "Test 5 Failed: int is not a base class");
static_assert(!is_base_of_v<void, B>, "Test 5 Failed: void is not a base class");

static_assert(std::is_base_of_v<PrivateBase, PrivateDerived>);
static_assert(!is_base_of_v<PrivateBase, PrivateDerived>, "SFINAE fails on private inheritance (Expected)");

static_assert(std::is_base_of_v<Base1, Diamond>);
static_assert(!is_base_of_v<Base1, Diamond>, "SFINAE fails on ambiguity (Expected)");

static_assert(is_same_v<int, int>);
static_assert(!is_same_v<int, double>);
static_assert(!is_same_v<float, double>);
static_assert(!is_same_v<int, int*>);

// --- Type Modifiers (Модификаторы типов) ---
static_assert(is_same_v<remove_reference_t<int*>, int*>);
static_assert(is_same_v<remove_reference_t<int>, int>);
static_assert(is_same_v<remove_reference_t<int&>, int>);
static_assert(is_same_v<remove_reference_t<int&&>, int>);

static_assert(is_same_v<remove_pointer_t<int*>, int>);
static_assert(is_same_v<remove_pointer_t<const int*>, const int>);
static_assert(is_same_v<remove_pointer_t<volatile int*>, volatile int>);
static_assert(is_same_v<remove_pointer_t<const volatile int*>, const volatile int>);
static_assert(is_same_v<remove_pointer_t<const volatile int&>, const volatile int&>);

static_assert(is_same_v<remove_const_t<int>, int>);
static_assert(is_same_v<remove_const_t<const int>, int>);
static_assert(is_same_v<remove_const_t<const volatile int>, volatile int>);
static_assert(is_same_v<remove_const_t<const volatile int*>, const volatile int*>);
static_assert(is_same_v<remove_const_t<volatile int* const>, volatile int*>);
static_assert(is_same_v<remove_const_t<const volatile int&>, const volatile int&>);

static_assert(is_same_v<remove_cv_t<int>, int>);
static_assert(is_same_v<remove_cv_t<const int>, int>);
static_assert(is_same_v<remove_cv_t<const volatile int>, int>);
static_assert(is_same_v<remove_cv_t<const volatile int*>, const volatile int*>);
static_assert(is_same_v<remove_cv_t<volatile int* const>, volatile int*>);
static_assert(is_same_v<remove_cv_t<int* volatile const>, int*>);
static_assert(is_same_v<remove_cv_t<const volatile int&>, const volatile int&>);

static_assert(is_same_v<add_pointer_t<int>, int*>);
static_assert(is_same_v<add_pointer_t<int*>, int**>);
static_assert(is_same_v<add_pointer_t<int**>, int***>);
static_assert(is_same_v<add_pointer_t<int&>, int*>);
static_assert(is_same_v<add_pointer_t<int*&>, int**>);
static_assert(is_same_v<add_pointer_t<int&&>, int*>);

// --- Type Categories (Категории типов) ---
static_assert(is_signed_v<int>);
static_assert(is_signed_v<char>);
static_assert(is_signed_v<float>);
static_assert(is_signed_v<double>);
static_assert(!is_signed_v<uint32_t>);
static_assert(!is_signed_v<unsigned char>);

static_assert(!is_unsigned_v<int>);
static_assert(!is_unsigned_v<char>);
static_assert(!is_unsigned_v<float>);
static_assert(is_unsigned_v<uint32_t>);
static_assert(is_unsigned_v<unsigned char>);

static_assert(is_void_v<void>);
static_assert(!is_void_v<int*>);

static_assert(is_nullptr_v<std::nullptr_t>);
static_assert(!is_nullptr_v<int*>);

static_assert(!is_pointer_v<int>);
static_assert(is_pointer_v<int*>);
static_assert(is_pointer_v<int* const>);

static_assert(!is_reference_v<int>);
static_assert(!is_reference_v<int*>);
static_assert(is_reference_v<int&>);
static_assert(is_reference_v<int&&>);
static_assert(is_reference_v<const int&&>);

static_assert(!is_const_v<int>);
static_assert(!is_const_v<volatile int>);
static_assert(!is_const_v<const volatile int*>);
static_assert(is_const_v<const volatile int>);
static_assert(is_const_v<volatile int* const>);

// --- Custom SFINAE / Concepts (Собственные трейты) ---
static_assert(imagine::has_member_foo<MyClass>::value);
static_assert(!imagine::has_member_foo<std::vector<int>>::value);
static_assert(!imagine::has_member_foo<int>::value);

static_assert(has_iterator_v<std::vector<int>>);
static_assert(has_iterator_v<MyContainer>);
static_assert(!has_iterator_v<EmptyStruct>);

static_assert(is_container_v<std::vector<int>>);
static_assert(!is_container_v<MyContainer>);
static_assert(is_container_v<EmptyStruct>);

// --- Type Transformations (Преобразования типов) ---
static_assert(is_same_v<conditional_t<true, std::vector<int>, std::list<int>>, std::vector<int>>);
static_assert(is_same_v<conditional_t<false, std::vector<int>, std::list<int>>, std::list<int>>);

static_assert(is_same_v<decay_t<const int&>, int>);
static_assert(is_same_v<decay_t<int[5]>, int*>);

static_assert(is_same_v<common_type_t<int, double>, double>);
static_assert(is_same_v<common_type_t<Base*, Derived*>, Base*>);
static_assert(is_same_v<common_type_t<Derived*, Base*>, Base*>);
static_assert(is_same_v<common_type_t<void*, int*>, void*>);
static_assert(is_same_v<common_type_t<decltype(nullptr), int*>, int*>);
static_assert(is_same_v<common_type_t<ConvertibleToInt, int>, int>);
static_assert(is_same_v<common_type_t<ConvertibleToInt, double>, double>);
static_assert(is_same_v<common_type_t<int[2], int[3]>, int*>);

// --- Operators & Properties (Операторы и свойства) ---
static_assert(is_addible_v<int>, "int + int should be true");
static_assert(is_addible_v<int, double>, "int + double should be true");
static_assert(!is_addible_v<int*>, "int* + int* should be false");
static_assert(is_addible_v<int*, int>, "int* + int should be true (pointer arithmetic)");

static_assert(is_equality_comparable_v<int>, "int should be comparable");
static_assert(is_equality_comparable_v<int, double>, "int and double should be comparable");

static_assert(is_assignable_v<int&, int>, "int can be assigned from int");
static_assert(!is_assignable_v<int&, void*>, "int cannot be assigned from void*");

static_assert(is_copy_assignable_v<int>, "int must be copy assignable");
static_assert(is_copy_assignable_v<double>, "double must be copy assignable");

// --- Logical Operators (Логические мета-операции) ---
static_assert(!negation_v<bool_constant<true>>);
static_assert(negation_v<bool_constant<false>>);

static_assert(conjunction_v<> == true, "Empty conjunction should be true");
static_assert(conjunction_v<True> == true);
static_assert(conjunction_v<False> == false);
static_assert(conjunction_v<True, True, True> == true);
static_assert(conjunction_v<True, True, False> == false);
static_assert(conjunction_v<False, ExplodingType<int>> == false, "Short-circuiting failed!");
static_assert(conjunction_v<true_type, std::is_integral<int>> == true);

static_assert(disjunction_v<> == false, "Empty disjunction should be false");
static_assert(disjunction_v<true_type> == true);
static_assert(disjunction_v<false_type> == false);
static_assert(disjunction_v<false_type, false_type, true_type> == true);

// --- Variadic Traits (Вариадические проверки) ---
static_assert(is_all_same_v<>);
static_assert(is_all_same_v<int, int, int, int>);
static_assert(!is_all_same_v<int, int, float>);
static_assert(!is_all_same_v<int, const int>);
static_assert(!is_all_same_v<int*, const int*>);

static_assert(is_all_same_decay_v<int, const int, volatile int, const volatile int>);
static_assert(is_all_same_decay_v<int, int&, int&&, const int&>);
static_assert(is_all_same_decay_v<int*, int[5], int[10], int[]>);
static_assert(is_all_same_decay_v<decltype(func), void (*)(int)>);
static_assert(!is_all_same_decay_v<int, long>);

}  // namespace

// ============================================================================

namespace {

// Тестовые объекты
struct MyClassFunc
{
    void method(int) {}

    int const_method(double, double) const
    {
        return 0;
    }
};

int free_function(const std::string&)
{
    return 42;
}

auto lambda = [](int a) -> double { return a * 1.5; };

}  // namespace

namespace {
struct ExplicitInt
{
    explicit ExplicitInt(int) {}
};

void target_derived(Derived*) {}

void target_explicit(ExplicitInt) {}

void target_rvalue(int&&) {}

void free_func_noexcept(int, float) noexcept {}

struct FullProcessor
{
    // 4 комбинации методов класса
    void m_plain() {}

    void m_const(int) const {}

    void m_noexcept(int, double) noexcept {}

    void m_const_noexcept(int, double, char) const noexcept {}
};

auto my_lambda = [](int a) -> double { return a * 1.5; };

}  // namespace

// ============================================================================
// Сами тесты
// ============================================================================

template <typename T>
using size_archetype = decltype(std::declval<T>().size());

void test_function_traits()
{
    // ==========================================
    // 1. ТЕСТЫ СВОБОДНЫХ ФУНКЦИЙ (СИГНАТУРЫ И УКАЗАТЕЛИ)
    // ==========================================

    // 1.1 Сигнатура обычной функции: R(Args...)
    using F_Sig = function_traits<void(int, float)>;
    static_assert(is_same_v<F_Sig::return_type, void>);
    static_assert(F_Sig::arity == 2);
    static_assert(!F_Sig::is_const);
    static_assert(!F_Sig::is_noexcept);
    static_assert(is_same_v<F_Sig::args_tuple, std::tuple<int, float>>);

    // 1.2 Сигнатура noexcept функции: R(Args...) noexcept
    using F_SigNoexcept = function_traits<void(int, float) noexcept>;
    static_assert(is_same_v<F_SigNoexcept::return_type, void>);
    static_assert(!F_SigNoexcept::is_const);
    static_assert(F_SigNoexcept::is_noexcept);

    // 1.3 Указатель на функцию: R(*)(Args...)
    using funcPtr = function_traits<decltype(&free_function)>;
    static_assert(is_same_v<funcPtr::return_type, int>);
    static_assert(funcPtr::arity == 1);
    static_assert(!funcPtr::is_const);
    static_assert(!funcPtr::is_noexcept);
    static_assert(is_same_v<funcPtr::arg_t<0>, const std::string&>);

    // 1.4 Указатель на noexcept функцию: R(*)(Args...) noexcept
    using F_PtrNoexcept = function_traits<decltype(&free_func_noexcept)>;
    static_assert(is_same_v<F_PtrNoexcept::return_type, void>);
    static_assert(!F_PtrNoexcept::is_const);
    static_assert(F_PtrNoexcept::is_noexcept);

    // ==========================================
    // 2. ТЕСТЫ МЕТОДОВ КЛАССА (ВСЕ 4 КОМБИНАЦИИ)
    // ==========================================

    // 2.1 Обычный метод (без const, без noexcept) + arity == 0
    using M_Plain = function_traits<decltype(&FullProcessor::m_plain)>;
    static_assert(is_same_v<M_Plain::class_type, FullProcessor>);
    static_assert(is_same_v<M_Plain::return_type, void>);
    static_assert(M_Plain::arity == 0);
    static_assert(!M_Plain::is_const);
    static_assert(!M_Plain::is_noexcept);
    static_assert(is_same_v<M_Plain::args_tuple, std::tuple<>>);  // Пустой tuple

    // 2.2 Const метод
    using M_Const = function_traits<decltype(&FullProcessor::m_const)>;
    static_assert(is_same_v<M_Const::class_type, FullProcessor>);
    static_assert(M_Const::arity == 1);
    static_assert(M_Const::is_const);
    static_assert(!M_Const::is_noexcept);
    static_assert(is_same_v<M_Const::arg_t<0>, int>);

    // 2.3 Noexcept метод
    using M_Noexcept = function_traits<decltype(&FullProcessor::m_noexcept)>;
    static_assert(is_same_v<M_Noexcept::class_type, FullProcessor>);
    static_assert(M_Noexcept::arity == 2);
    static_assert(!M_Noexcept::is_const);
    static_assert(M_Noexcept::is_noexcept);
    static_assert(is_same_v<M_Noexcept::args_tuple, std::tuple<int, double>>);

    // 2.4 Const Noexcept метод
    using M_ConstNoexcept = function_traits<decltype(&FullProcessor::m_const_noexcept)>;
    static_assert(is_same_v<M_ConstNoexcept::class_type, FullProcessor>);
    static_assert(M_ConstNoexcept::arity == 3);
    static_assert(M_ConstNoexcept::is_const);
    static_assert(M_ConstNoexcept::is_noexcept);
    static_assert(is_same_v<M_ConstNoexcept::arg_t<2>, char>);

    // ==========================================
    // 3. ТЕСТЫ ЛЯМБД И СЛОЖНЫХ ТИПОВ
    // ==========================================

    // 3.1 Лямбда (проверяем operator() - обычно он const по умолчанию)
    using L_Traits = function_traits<decltype(&decltype(my_lambda)::operator())>;
    static_assert(is_same_v<L_Traits::return_type, double>);
    static_assert(L_Traits::arity == 1);
    static_assert(L_Traits::is_const);  // operator() лямбды (без mutable) является const

    // 3.2 Сложная сигнатура с указателями на указатели
    using ComplexSig = function_traits<std::string*(const char**, int)>;
    static_assert(is_same_v<ComplexSig::return_type, std::string*>);
    static_assert(ComplexSig::arity == 2);
    static_assert(is_same_v<ComplexSig::arg_t<0>, const char**>);

    //////////////////////////////////

    static_assert(is_invokable_v<decltype(target_derived), Base*> == false, "Error 1");
    static_assert(
        is_invokable_v<decltype(target_derived), decltype(static_cast<Derived*>(std::declval<Base*>()))> == true, "Error 2");

    // 2. Explicit конструктор: int -> ExplicitInt
    // Неявно — нельзя, через static_cast — можно.
    static_assert(is_invokable_v<decltype(target_explicit), int> == false, "Error 3");
    static_assert(
        is_invokable_v<decltype(target_explicit), decltype(static_cast<ExplicitInt>(std::declval<int>()))> == true, "Error 4");

    // 3. Ссылки: Lvalue -> Rvalue (имитация std::move)
    // Неявно — нельзя передать x в int&&, через static_cast — можно.
    static_assert(is_invokable_v<decltype(target_rvalue), int&> == false, "Error 5");
    static_assert(is_invokable_v<decltype(target_rvalue), decltype(static_cast<int&&>(std::declval<int&>()))> == true, "Error 6");

    static_assert(is_detected_v<size_archetype, std::vector<int>>, "Vector has size()");
    static_assert(is_detected_v<size_archetype, std::string>, "String has size()");
    static_assert(!is_detected_v<size_archetype, int>, "Int has no size()");

    static_assert(args_count_v<> == 0, "Error: count of empty args should be 0");
    static_assert(args_count_v<int> == 1, "Error: count of <int> should be 1");
    static_assert(args_count_v<int, char, double, float> == 4, "Error: count of 4 types should be 4");
    static_assert(args_count_v<int, int, int> == 3, "Error: count of <int, int, int> should be 3");
    static_assert(args_count_v<int&, const double*, void (*)(), int[10]> == 4, "Error: complex types failed");
}

int main()
{
    // Поскольку все проверки происходят на этапе компиляции,
    // main() остается чистым. Если код скомпилировался — тесты пройдены!
    return 0;
}
