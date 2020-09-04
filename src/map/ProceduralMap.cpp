//
// Created by guillaumeg on 29/05/2020.
//

#include <iostream>
#include <functional>
#include <utils/MapException.hpp>
#include <map/ProceduralMap.hpp>

void ProceduralMap::applyBlocs()
{
    std::random_device rd;
    std::mt19937 re(rd());
    std::uniform_int_distribution<int> distribution{0, 2};

    for (ushort i = 1; i < _size.second - 1; i++)
        for (ushort j = 1; j < _size.first - 1; j++)
            _map[getIndex(j, i)] =
                (i % 2) && (j % 2) ? ProceduralMap::Unbreakable : (distribution(re) ? ProceduralMap::Breakable : ProceduralMap::Walkable);
    for (ushort i = 2; i < _size.second - 2; i++)
        for (ushort j = 0; j < _size.first; j += _size.first - 1)
            _map[getIndex(j, i)] = ProceduralMap::Breakable;
    for (ushort i = 0; i < _size.second; i += _size.second - 1)
        for (ushort j = 2; j < _size.first - 2; j++)
            _map[getIndex(j, i)] = ProceduralMap::Breakable;
    computePowerUp(re);
}

ProceduralMap::ProceduralMap() : _map(nullptr)
{
}

ProceduralMap::ProceduralMap(std::pair<ushort, ushort> size) : _size(std::move(size))
{
    if (_size.first % 2 == 0 || _size.second % 2 == 0)
        throw MapException(__FILE__, __LINE__, SIZE);
    _map = new uint[(int)_size.second * _size.first];
    for (int i = 0; i < _size.second * _size.first; i++)
        _map[i] = 0;
    applyBlocs();
}

ProceduralMap::ProceduralMap(ushort x, ushort y) : ProceduralMap(std::make_pair(x, y))
{
}

ProceduralMap::~ProceduralMap()
{
    delete[] _map;
}

uint ProceduralMap::operator[](uint position) const
{
    if (position >= (uint)(_size.first * _size.second))
        throw MapException(__FILE__, __LINE__, OUT_OF_BOUNDS);
    return _map[position];
}

void ProceduralMap::printInTerminal() const noexcept
{
    for (ushort i = 0; i < _size.second * _size.first; i++)
    {
        std::cout << _map[i];
        if (i && !((i + 1) % _size.first))
            std::cout << std::endl;
    }
}

uint ProceduralMap::getIndex(ushort x, ushort y) const noexcept
{
    return _size.first * y + x;
}

uint ProceduralMap::at(uint position) const
{
    if (position >= (uint)(_size.first * _size.second))
        throw MapException(__FILE__, __LINE__, OUT_OF_BOUNDS);
    return _map[position];
}

uint ProceduralMap::computePowerUp(std::mt19937 gen)
{
    std::uniform_int_distribution<int> placeBlockProbability{1, 10};
    std::uniform_int_distribution<int> powerUp{1, 100};
    double placed = 0;
    double nb = 0;
    for (int i = 0; i < _size.first * _size.second; i++)
        if (_map[i] == 0)
            nb += 1.0;
    double blockWithItem = std::round(nb / 100 * 40);
    while (placed < blockWithItem / 2)
        for (int i = 0; i < _size.first * _size.second; i++)
        {
            if (_map[i] == ProceduralMap::Breakable && placeBlockProbability(gen) == 1)
            {
                placed++;
                int r = powerUp(gen);
                _map[i] = r <= 10 ? 6 : r <= 30 ? 5 : r <= 65 ? 4 : 3;
            }
        }
    while (placed < blockWithItem)
        for (int i = _size.first * _size.second - 1; i >= 0; i--)
        {
            if (_map[i] == ProceduralMap::Breakable && placeBlockProbability(gen) == 1)
            {
                placed++;
                int r = powerUp(gen);
                _map[i] = r <= 10 ? 6 : r <= 30 ? 5 : r <= 65 ? 4 : 3;
            }
        }
    return 0;
}

