# [GenLinearHierarchy](GenLinearHierarchy.hpp)

Given a class template `T` and a typelist `List`, generate a new type inheriting from `T` specialized on each type in `List`.

## Definition

```
template<
    template<class> class T,
    typename U,
    typename ...TList>
class GenLinearHierarchy<T, U, TList...>
    :
    public T<U>,
    public GenLinearHierarchy<T, TList...>
{
};
```
