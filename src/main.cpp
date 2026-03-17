#include "type_traits.hpp"

#include <list>
#include <string>
#include <vector>

#include <type_traits>

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

namespace {
struct Base
{};

struct Derived: Base
{};

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
}  // namespace

int main()
{
    static_assert(traits::is_same_v<traits::remove_reference_t<int*>, int*>);
    static_assert(traits::is_same_v<traits::remove_reference_t<int>, int>);
    static_assert(traits::is_same_v<traits::remove_reference_t<int&>, int>);
    static_assert(traits::is_same_v<traits::remove_reference_t<int&&>, int>);

    static_assert(traits::is_same_v<traits::remove_pointer_t<int*>, int>);
    static_assert(traits::is_same_v<traits::remove_pointer_t<const int*>, const int>);
    static_assert(traits::is_same_v<traits::remove_pointer_t<volatile int*>, volatile int>);
    static_assert(traits::is_same_v<traits::remove_pointer_t<const volatile int*>, const volatile int>);
    static_assert(traits::is_same_v<traits::remove_pointer_t<const volatile int&>, const volatile int&>);

    static_assert(traits::is_same_v<traits::remove_const_t<int>, int>);
    static_assert(traits::is_same_v<traits::remove_const_t<const int>, int>);
    static_assert(traits::is_same_v<traits::remove_const_t<const volatile int>, volatile int>);
    static_assert(traits::is_same_v<traits::remove_const_t<const volatile int*>, const volatile int*>);
    static_assert(traits::is_same_v<traits::remove_const_t<volatile int* const>, volatile int*>);
    static_assert(traits::is_same_v<traits::remove_const_t<const volatile int&>, const volatile int&>);

    static_assert(traits::is_same_v<traits::remove_cv_t<int>, int>);
    static_assert(traits::is_same_v<traits::remove_cv_t<const int>, int>);
    static_assert(traits::is_same_v<traits::remove_cv_t<const volatile int>, int>);
    static_assert(traits::is_same_v<traits::remove_cv_t<const volatile int*>, const volatile int*>);
    static_assert(traits::is_same_v<traits::remove_cv_t<volatile int* const>, volatile int*>);
    static_assert(traits::is_same_v<traits::remove_cv_t<int* volatile const>, int*>);
    static_assert(traits::is_same_v<traits::remove_cv_t<const volatile int&>, const volatile int&>);

    static_assert(traits::is_same_v<traits::add_pointer_t<int>, int*>);
    static_assert(traits::is_same_v<traits::add_pointer_t<int*>, int**>);
    static_assert(traits::is_same_v<traits::add_pointer_t<int**>, int***>);
    static_assert(traits::is_same_v<traits::add_pointer_t<int&>, int*>);
    static_assert(traits::is_same_v<traits::add_pointer_t<int*&>, int**>);
    static_assert(traits::is_same_v<traits::add_pointer_t<int&&>, int*>);

    ////////////////////////

    static_assert(traits::is_signed_v<int>);
    static_assert(traits::is_signed_v<char>);
    static_assert(traits::is_signed_v<float>);
    static_assert(traits::is_signed_v<double>);
    static_assert(!traits::is_signed_v<uint32_t>);
    static_assert(!traits::is_signed_v<unsigned char>);
    static_assert(!traits::is_signed_v<unsigned short>);
    static_assert(!traits::is_signed_v<unsigned int>);
    static_assert(!traits::is_signed_v<unsigned long>);
    static_assert(!traits::is_signed_v<unsigned long long>);

    static_assert(!traits::is_unsigned_v<int>);
    static_assert(!traits::is_unsigned_v<char>);
    static_assert(!traits::is_unsigned_v<float>);
    static_assert(!traits::is_unsigned_v<double>);
    static_assert(traits::is_unsigned_v<uint32_t>);
    static_assert(traits::is_unsigned_v<unsigned char>);
    static_assert(traits::is_unsigned_v<unsigned short>);
    static_assert(traits::is_unsigned_v<unsigned int>);
    static_assert(traits::is_unsigned_v<unsigned long>);
    static_assert(traits::is_unsigned_v<unsigned long long>);

    static_assert(traits::is_same_v<int, int>);
    static_assert(!traits::is_same_v<int, double>);
    static_assert(!traits::is_same_v<float, double>);
    static_assert(!traits::is_same_v<int, int*>);

    static_assert(traits::is_void_v<void>);
    static_assert(!traits::is_void_v<int*>);

    static_assert(traits::is_nullptr_v<nullptr_t>);
    static_assert(!traits::is_nullptr_v<int*>);

    static_assert(!traits::is_pointer_v<int>);
    static_assert(traits::is_pointer_v<int*>);
    static_assert(traits::is_pointer_v<int* const>);

    static_assert(!traits::is_reference_v<int>);
    static_assert(!traits::is_reference_v<int*>);
    static_assert(traits::is_reference_v<int&>);
    static_assert(traits::is_reference_v<int&&>);
    static_assert(traits::is_reference_v<const int&&>);
    static_assert(traits::is_reference_v<const volatile int&&>);

    static_assert(!traits::is_const_v<int>);
    static_assert(!traits::is_const_v<volatile int>);
    static_assert(!traits::is_const_v<const volatile int*>);
    static_assert(traits::is_const_v<const volatile int>);
    static_assert(traits::is_const_v<volatile int* const>);
    static_assert(traits::is_const_v<int* volatile const>);

    ////////////////////////

    static_assert(traits::imagine::has_member_foo<test_classes::MyClass>::value);
    static_assert(!traits::imagine::has_member_foo<std::vector<int>>::value);
    static_assert(!traits::imagine::has_member_foo<int>::value);

    ////////////////////////

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

    static_assert(traits::has_iterator_v<std::vector<int>>);
    static_assert(traits::has_iterator_v<MyContainer>);
    static_assert(!traits::has_iterator_v<EmptyStruct>);

    ////////////////////////

    static_assert(traits::is_container_v<std::vector<int>>);
    static_assert(!traits::is_container_v<MyContainer>);
    static_assert(traits::is_container_v<EmptyStruct>);

    ////////////////////////

    static_assert(traits::is_same_v<traits::conditional_t<true, std::vector<int>, std::list<int>>, std::vector<int>>);
    static_assert(traits::is_same_v<traits::conditional_t<false, std::vector<int>, std::list<int>>, std::list<int>>);

    static_assert(traits::is_same_v<traits::decay_t<const int&>, int>);

    // 2. Массивы (сработает ветка is_array_v)
    static_assert(traits::is_same_v<traits::decay_t<int[5]>, int*>);

    ////////////////////////

    static_assert(traits::is_same_v<traits::common_type_t<int, double>, double>);

    static_assert(traits::is_same_v<traits::common_type_t<Base*, Derived*>, Base*>);
    static_assert(traits::is_same_v<traits::common_type_t<Derived*, Base*>, Base*>);
    static_assert(traits::is_same_v<std::common_type_t<Derived*, Derived*>, Derived*>);
    static_assert(traits::is_same_v<traits::common_type_t<void*, int*>, void*>);
    static_assert(traits::is_same_v<traits::common_type_t<decltype(nullptr), int*>, int*>);
    static_assert(traits::is_same_v<traits::common_type_t<Base*, decltype(nullptr)>, Base*>);

    static_assert(traits::is_same_v<traits::common_type_t<ConvertibleToInt, int>, int>);
    static_assert(traits::is_same_v<traits::common_type_t<int, ConvertibleToInt>, int>);
    static_assert(traits::is_same_v<traits::common_type_t<ConvertibleToInt, double>, double>);

    static_assert(traits::is_same_v<traits::common_type_t<int[2], int[3]>, int*>);
    static_assert(traits::is_same_v<traits::common_type_t<const int[2], int[3]>, const int*>);

    static_assert(traits::negation_v<traits::bool_constant<true>> == false);
    static_assert(traits::negation_v<traits::bool_constant<false>> == true);

    return 0;
}
