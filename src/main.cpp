#include "type_traits.hpp"

int main()
{
    static_assert(!traits::IsPointerV<int>);
    static_assert(traits::IsPointerV<int*>);

    return 0;
}
