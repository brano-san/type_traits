#include "type_traits.hpp"

#include <string>
#include <vector>

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

int main()
{
    static_assert(std::is_same_v<traits::remove_reference_t<int*>, int*>);
    static_assert(std::is_same_v<traits::remove_reference_t<int>, int>);
    static_assert(std::is_same_v<traits::remove_reference_t<int&>, int>);
    static_assert(std::is_same_v<traits::remove_reference_t<int&&>, int>);

    static_assert(std::is_same_v<traits::remove_pointer_t<int*>, int>);
    static_assert(std::is_same_v<traits::remove_pointer_t<const int*>, const int>);
    static_assert(std::is_same_v<traits::remove_pointer_t<volatile int*>, volatile int>);
    static_assert(std::is_same_v<traits::remove_pointer_t<const volatile int*>, const volatile int>);
    static_assert(std::is_same_v<traits::remove_pointer_t<const volatile int&>, const volatile int&>);

    static_assert(std::is_same_v<traits::remove_const_t<int>, int>);
    static_assert(std::is_same_v<traits::remove_const_t<const int>, int>);
    static_assert(std::is_same_v<traits::remove_const_t<const volatile int>, volatile int>);
    static_assert(std::is_same_v<traits::remove_const_t<const volatile int*>, const volatile int*>);
    static_assert(std::is_same_v<traits::remove_const_t<volatile int* const>, volatile int*>);
    static_assert(std::is_same_v<traits::remove_const_t<const volatile int&>, const volatile int&>);

    static_assert(std::is_same_v<traits::remove_cv_t<int>, int>);
    static_assert(std::is_same_v<traits::remove_cv_t<const int>, int>);
    static_assert(std::is_same_v<traits::remove_cv_t<const volatile int>, int>);
    static_assert(std::is_same_v<traits::remove_cv_t<const volatile int*>, const volatile int*>);
    static_assert(std::is_same_v<traits::remove_cv_t<volatile int* const>, volatile int*>);
    static_assert(std::is_same_v<traits::remove_cv_t<int* volatile const>, int*>);
    static_assert(std::is_same_v<traits::remove_cv_t<const volatile int&>, const volatile int&>);

    static_assert(std::is_same_v<traits::add_pointer_t<int>, int*>);
    static_assert(std::is_same_v<traits::add_pointer_t<int*>, int**>);
    static_assert(std::is_same_v<traits::add_pointer_t<int**>, int***>);
    static_assert(std::is_same_v<traits::add_pointer_t<int&>, int*>);
    static_assert(std::is_same_v<traits::add_pointer_t<int*&>, int**>);
    static_assert(std::is_same_v<traits::add_pointer_t<int&&>, int*>);

    ////////////////////////

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

    static_assert(traits::has_iterator_v<std::vector<int>>, "Vector should have iterator");
    static_assert(traits::has_iterator_v<MyContainer>, "MyContainer should have iterator");
    static_assert(!traits::has_iterator_v<EmptyStruct>, "EmptyStruct should not have iterator");

    ////////////////////////

    static_assert(traits::is_container_v<std::vector<int>>, "Vector should have iterator");
    static_assert(!traits::is_container_v<MyContainer>, "MyContainer should have iterator");
    static_assert(traits::is_container_v<EmptyStruct>, "EmptyStruct should not have iterator");

    return 0;
}
