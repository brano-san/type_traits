#include "type_traits.hpp"

int main()
{
    static_assert(std::is_same_v<traits::RemoveReferenceT<int*>, int*>);
    static_assert(std::is_same_v<traits::RemoveReferenceT<int>, int>);
    static_assert(std::is_same_v<traits::RemoveReferenceT<int&>, int>);
    static_assert(std::is_same_v<traits::RemoveReferenceT<int&&>, int>);

    static_assert(std::is_same_v<traits::RemovePointerT<int*>, int>);
    static_assert(std::is_same_v<traits::RemovePointerT<const int*>, const int>);
    static_assert(std::is_same_v<traits::RemovePointerT<volatile int*>, volatile int>);
    static_assert(std::is_same_v<traits::RemovePointerT<const volatile int*>, const volatile int>);
    static_assert(std::is_same_v<traits::RemovePointerT<const volatile int&>, const volatile int&>);

    static_assert(std::is_same_v<traits::RemoveConstT<int>, int>);
    static_assert(std::is_same_v<traits::RemoveConstT<const int>, int>);
    static_assert(std::is_same_v<traits::RemoveConstT<const volatile int>, volatile int>);
    static_assert(std::is_same_v<traits::RemoveConstT<const volatile int*>, const volatile int*>);
    static_assert(std::is_same_v<traits::RemoveConstT<volatile int* const>, volatile int*>);
    static_assert(std::is_same_v<traits::RemoveConstT<const volatile int&>, const volatile int&>);

    static_assert(std::is_same_v<traits::RemoveCvT<int>, int>);
    static_assert(std::is_same_v<traits::RemoveCvT<const int>, int>);
    static_assert(std::is_same_v<traits::RemoveCvT<const volatile int>, int>);
    static_assert(std::is_same_v<traits::RemoveCvT<const volatile int*>, const volatile int*>);
    static_assert(std::is_same_v<traits::RemoveCvT<volatile int* const>, volatile int*>);
    static_assert(std::is_same_v<traits::RemoveCvT<int* volatile const>, int*>);
    static_assert(std::is_same_v<traits::RemoveCvT<const volatile int&>, const volatile int&>);

    ////////////////////////

    static_assert(traits::IsSameV<int, int>);
    static_assert(!traits::IsSameV<int, double>);
    static_assert(!traits::IsSameV<float, double>);
    static_assert(!traits::IsSameV<int, int*>);

    static_assert(traits::IsVoidV<void>);
    static_assert(!traits::IsVoidV<int*>);

    static_assert(traits::IsNullptrV<nullptr_t>);
    static_assert(!traits::IsNullptrV<int*>);

    static_assert(!traits::IsPointerV<int>);
    static_assert(traits::IsPointerV<int*>);

    static_assert(!traits::IsReferenceV<int>);
    static_assert(!traits::IsReferenceV<int*>);
    static_assert(traits::IsReferenceV<int&>);
    static_assert(traits::IsReferenceV<int&&>);
    static_assert(traits::IsReferenceV<const int&&>);
    static_assert(traits::IsReferenceV<const volatile int&&>);

    static_assert(!traits::IsConstV<int>);
    static_assert(!traits::IsConstV<volatile int>);
    static_assert(!traits::IsConstV<const volatile int*>);
    static_assert(traits::IsConstV<const volatile int>);
    static_assert(traits::IsConstV<volatile int* const>);
    static_assert(traits::IsConstV<int* volatile const>);

    return 0;
}
