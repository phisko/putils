# [AStar](AStar.hpp)

An implementation of the AStar algorithm.

### Members

##### getNextDirection

```cpp
Direction getNextDirection(const Point<Precision> &start, const Point<Precision> &goal,
                           const std::function<bool(const Point<Precision> &dest)> &canMoveTo);
```
Returns the next move to perform in order to go from `start` to `goal`.

* `start`: x and y position where character currently is
* `goal`: destination the character would like to eventually reach
* `canMoveTo`: predicate indicating whether given coordinates can be visited (returns false if `dest` is a wall, for instance)
