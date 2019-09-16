# [AbstractFactory<TList...>](AbstractFactory.hpp)

Template class for an abstract factory able to create any type in a provided typelist.
Given an `AbstractFactory<TList...>`, users can call `make<T>()` to create an object of type T (or any type derived from it, as determined by the concrete factory implementation).

### Members

##### make
```cpp
template<typename T> // T must be in TList
std::unique_ptr<T> make();
```
Creates an object of type T, or any type derived from T. Construction is performed by a class derived from AbstractFactory (and therefore implementation defined).

### Example code
In this example, "CFactory" can create `ints` and `doubles`, which isn't particularly useful. However, you can imagine how this could be useful if CFactory could create concrete versions of GUI elements, for instance.

```cpp
#include <iostream>
#include "AbstractFactory.hpp"

int main()
{
    class CFactory : public AbstractFactory<int, double>
    {
        std::unique_ptr<int> makeImpl(pmeta::type<int>) noexcept final { return std::make_unique<int>(0); }
        std::unique_ptr<double> makeImpl(pmeta::type<double>) noexcept final { return std::make_unique<double>(42); }
    };

    CFactory factory;

    auto ptrToInt = factory.make<int>();
    auto ptrToDouble = factory.make<double>();

    assert(*i == 0);
    assert(*d == 42);
}
```
