#pragma once

#include <functional>
#include <vector>
#include <algorithm>

#include "Point.hpp"

namespace putils {
    template<typename Contained>
    struct Cell {
        Point<int, 3> pos;
        Contained obj;
    };

    template<typename Contained>
    class CellularAutomaton {
    public:
        using NeighborFetcher = std::function<std::vector<const Cell<Contained> *>(const std::vector<Cell<Contained>> & cells, const Point<int, 3> & pos)>;
        using ModifierCondition = std::function<bool(const std::vector<const Cell<Contained> *> & neighbors, const Cell<Contained> & cell)>;
        using Modifier = std::function<void(Cell<Contained> & cell)>;

        // Ctors
    public:
        CellularAutomaton(std::vector<Cell<Contained>> && cells, std::size_t dimensions = 2);

        CellularAutomaton(const putils::Point<std::size_t> & size, const Contained & value = Contained{},
                          const putils::Point<int> & start = { 0, 0 }, std::size_t dimensions = 2);

        // Cells getters and setters
    public:
        const std::vector<Cell<Contained>> & getCells() const noexcept { return _cells; }
        std::vector<Cell<Contained>> & getCells() noexcept { return _cells; }


        // Apply modifier to all cells for which condition is true
    public:
        const std::vector<Cell<Contained>> & step(
                const ModifierCondition & condition,
                const Modifier & modifier,
                const NeighborFetcher & fetcher = nullptr) noexcept;



        /*
         * Implementaion details
         */

    private:
        std::vector<const Cell<Contained> *> getNeighbors(const Point<int, 3> & pos) const noexcept;

        static bool x_neighbor(const Point<int, 3> & a, const Point<int, 3> & b) {
            return (a.x == b.x - 1 || a.x == b.x + 1) && a.y == b.y && a.z == b.z;
        }

        static bool y_neighbor(const Point<int, 3> & a, const Point<int, 3> & b) {
            return a.x == b.x && (a.y == b.y - 1 || a.y == b.y + 1) && a.z == b.z;
        }

        static bool z_neighbor(const Point<int, 3> & a, const Point<int, 3> & b) {
            return a.x == b.x && a.y == b.y && (a.z == b.z - 1 || a.z == b.z + 1);
        }

    private:
        std::vector<Cell<Contained>> _cells;
        std::size_t _dimensions;
    };

    /*
     * Ctors
     */

    template<typename Contained>
    CellularAutomaton<Contained>::CellularAutomaton(std::vector<Cell<Contained>> && cells, std::size_t dimensions)
            : _cells(std::move(cells)), _dimensions(dimensions) {
    }

    template<typename Contained>
    CellularAutomaton<Contained>::CellularAutomaton(
            const putils::Point<std::size_t> & size,
            const Contained & value,
            const putils::Point<int> & start,
            std::size_t dimensions)
            : _dimensions(dimensions) {
        for (std::size_t x = 0; x < size.x; ++x)
            for (std::size_t y = 0; y < size.y; ++y)
                _cells.push_back({ { (int) x + start.x, (int) y + start.y }, value });
    }


    /*
     * Step
     */

    template<typename Contained>
    std::vector<const Cell<Contained> *> CellularAutomaton<Contained>::getNeighbors(const Point<int, 3> & pos) const noexcept {
        std::vector<const Cell<Contained> *> ret;

        for (const auto & c : _cells)
            if ((x_neighbor(c.pos, pos) || y_neighbor(c.pos, pos)) &&
                _dimensions == 2 || y_neighbor(c.pos, pos))
                ret.push_back(&c);

        return ret;
    }

    template<typename Contained>
    const std::vector<Cell<Contained>> & CellularAutomaton<Contained>::step(
            const ModifierCondition & condition,
            const Modifier & modifier,
            const NeighborFetcher & fetcher) noexcept {
        std::vector<Cell<Contained>> _newCells(_cells);

        for (auto & cell : _newCells) {
            const auto neighbors =
                    (fetcher == nullptr) ? getNeighbors(cell.pos) : fetcher(_cells, cell.pos);

            if (condition(neighbors, cell))
                modifier(cell);
        }

        _cells = std::move(_newCells);
        return _cells;
    }
}
