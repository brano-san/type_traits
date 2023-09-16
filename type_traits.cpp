#include <iostream>
#include <vector>
#include <list>
#include <set>

using namespace std;

template<typename T>
class HasBeginEnd
{
private:
    template<class U>
    static constexpr auto cheker(U* u) -> decltype((*u).begin(), (*u).end(), bool()) { return true; }

    template<class>
    static constexpr auto cheker(...) { return false; }

public:
    static bool constexpr value = cheker<T>(nullptr);
};

template<typename T>
class HasConstBeginEnd
{
private:
    template<class U>
    static constexpr auto constCheker(U* u) -> decltype((*u).cbegin(), (*u).cend(), bool()) { return true; }

    template<class>
    static constexpr auto constCheker(...) { return false; }

public:
    static bool constexpr value = constCheker<T>(nullptr);
};

template<typename T, typename U>
class IsSame
{
public:
    static bool constexpr value = false;
};

template<typename T>
class IsSame<T, T>
{
public:
    static bool constexpr value = true;
};

class MyClass
{
public:

};

int main()
{
    std::is_same<int, float>::value_type;
    cout << HasBeginEnd<vector<int>>::value << endl;
    cout << HasBeginEnd<list<int>>::value << endl;
    cout << HasBeginEnd<MyClass>::value << endl;
}
