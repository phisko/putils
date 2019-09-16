# [CellularAutomaton](CellularAutomaton.hpp)

A simple implementation for a generic cellular automaton.

### Members

##### Constructors

```cpp
CellularAutomaton(std::vector<Cell<Contained>> &&cells, std::size_t dimensions = 2);
```
Constructs a `CellularAutomaton` using the provided set of cells.

```cpp
CellularAutomaton(const putils::Point<std::size_t> &size, const Contained &value = Contained{},
                  const putils::Point<int> &start = { 0, 0 }, std::size_t dimensions = 2);
```
Constructs a `CellularAutomaton` of the given `size`, constructing cells by copying `value`.

##### getCells

```cpp
const std::vector<Cell<Contained>> &getCells() const { return _cells; }
std::vector<Cell<Contained>> &getCells() { return _cells; }
```
Provides access to the set of cells. Non-const getter is provided, but should not be used (see `step`)

##### step

```cpp
const std::vector<Cell<Contained>> &step(
        const ModifierCondition &condition,
        const Modifier &modifier,
        const NeighborFetcher &fetcher = nullptr);
```
Walks through the set of cells and applies `modifier` to all cells for which `condition` is true.

`fetcher` has a default value which returns the 4 cells directly next to the current one.

Returns the set of cells after modification.

See `Function types` for the types of the parameters.

##### Function types
```cpp
using NeighborFetcher = std::function<std::vector<const Cell<Contained> *>(const std::vector<Cell<Contained>> &cells, const Point<int, 3> &pos)>;
```
`NeighborFetcher` defines a function which, given a certain cell, returns its neighbors.

```cpp
using ModifierCondition = std::function<bool(const std::vector<const Cell<Contained> *> &neighbors, const Cell<Contained> &cell)>;
```
`ModifierCondition` defines a function which, given a certain cell and its neighbors, returns whether the cell should be modified.

```cpp
using Modifier = std::function<void(Cell<Contained> &cell)>;
```
`Modifier` defines a function which, given a certain cell, modifies it.

### Example

```cpp
int main()
{
    CellularAutomaton<bool> ca(
        {
           { { 0 }, false },
           { { 1 }, false },
           { { 2 }, true },
           { 3 }, false },
           { { 4 }, false },
        }
    );

    const auto &cells = ca.step(
        [](const std::vector<const Cell<bool> *> &neighbors, const Cell<bool> &cell)
        {
            return std::find_if(neighbors.begin(), neighbors.end(), [](auto &&ptr){ return ptr->obj; })
                != neighbors.end();
        },
        [](Cell<bool> &cell)
        {
            cell.obj = !cell.obj;
        }
    );

    for (const auto &c : cells)
        std::cout << std::boolalpha << c.obj << std::endl;
}
```
