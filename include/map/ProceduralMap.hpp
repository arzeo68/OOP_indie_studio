//
// Created by guillaumeg on 29/05/2020.
//

#ifndef MAPPROCESS_PROCEDURALMAP_HPP
#define MAPPROCESS_PROCEDURALMAP_HPP

#include <random>
#include <chrono>
#include <utility>
#include <vector>

using ushort = unsigned short;
using uint = unsigned int;

class ProceduralMap
{
    public:
    enum Area
    {
        Walkable,
        Unbreakable,
        Breakable
    };

    ProceduralMap();

    ProceduralMap(ushort, ushort);

    ProceduralMap(ProceduralMap &&) = default;

    explicit ProceduralMap(std::pair<ushort, ushort> coord);

    ~ProceduralMap();

    ProceduralMap(const ProceduralMap &) = default;

    ProceduralMap &operator=(const ProceduralMap &) = default;

    void printInTerminal() const noexcept;

    uint operator[](uint position) const;

    [[nodiscard]] uint at(uint position) const;

    uint computePowerUp(std::mt19937 re);

    [[nodiscard]] uint *getMap()
    {
        return _map;
    }

    private:
    std::pair<ushort, ushort> _size;
    uint *_map;

    void applyBlocs();

    [[nodiscard]] uint getIndex(ushort x, ushort y) const noexcept;
};

#define SIZE "one of the size is equal or inferior at 0 !"
#define OUT_OF_BOUNDS "the position you gave is out of bounds !"

#endif //MAPPROCESS_PROCEDURALMAP_HPP
