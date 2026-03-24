#include "type_traits.hpp"

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

int complex_func(double a, std::string b, char c)
{
    return 0;
}

// Тестовый класс
struct Processor
{
    void process(int x, float y) const {}
};

void test_function_traits()
{
    using namespace traits;

    // 1. Тест обычной сигнатуры функции
    using T1 = function_traits<void(int, float)>;
    static_assert(is_same_v<typename T1::return_type, void>);
    static_assert(T1::arity == 2);

    // 2. Тест указателя на свободную функцию
    using T2 = function_traits<decltype(&free_function)>;
    static_assert(is_same_v<typename T2::return_type, int>);
    static_assert(T2::arity == 1);

    // 3. Тест обычного метода класса
    using T3 = function_traits<decltype(&MyClassFunc::method)>;
    static_assert(is_same_v<typename T3::return_type, void>);
    static_assert(is_same_v<typename T3::class_type, MyClassFunc>);
    static_assert(T3::arity == 1);

    // 4. Тест константного метода класса
    using T4 = function_traits<decltype(&MyClassFunc::const_method)>;
    static_assert(is_same_v<typename T4::return_type, int>);
    static_assert(is_same_v<typename T4::class_type, MyClassFunc>);
    static_assert(T4::arity == 2);

    // 5. Тест лямбды (через ее operator())
    // Лямбда сама по себе не подходит под шаблоны выше,
    // но можно проверить её метод вызова:
    using T5 = function_traits<decltype(&decltype(lambda)::operator())>;
    static_assert(is_same_v<typename T5::return_type, double>);
    static_assert(T5::arity == 1);

    // 6. Сложные типы
    using T6 = function_traits<std::string*(const char**, int)>;
    static_assert(is_same_v<typename T6::return_type, std::string*>);
    static_assert(T6::arity == 2);

    static_assert(is_same_v<argument_t<2, int, double, float, char>, float>);

    using F1 = function_traits<decltype(complex_func)>;
    static_assert(std::is_same_v<F1::return_type, int>, "Return type mismatch");
    static_assert(F1::arity == 3, "Arity mismatch");

    static_assert(std::is_same_v<F1::arg_t<0>, double>, "Arg 0 mismatch");
    static_assert(std::is_same_v<F1::arg_t<1>, std::string>, "Arg 1 mismatch");
    static_assert(std::is_same_v<F1::arg_t<2>, char>, "Arg 2 mismatch");

    // Тест для метода класса
    using F2 = function_traits<decltype(&Processor::process)>;
    static_assert(std::is_same_v<F2::class_type, Processor>, "Class type mismatch");
    static_assert(std::is_same_v<F2::arg_t<0>, int>, "Method Arg 0 mismatch");
    static_assert(std::is_same_v<F2::arg_t<1>, float>, "Method Arg 1 mismatch");
    static_assert(std::is_same_v<F2::return_type, void>);
}

int main()
{
    // Поскольку все проверки происходят на этапе компиляции,
    // main() остается чистым. Если код скомпилировался — тесты пройдены!
    return 0;
}
