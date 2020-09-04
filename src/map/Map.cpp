//
// Created by alexis on 27/05/2020.
//

#include <engine/Irrlicht.hpp>
#include <map/Map.hpp>

Map::Map(const std::shared_ptr<Irrlicht> &irr) : Map(15, 11, (float)1.2, irr)
{
    createExternalWalls(irr);
    placeObstacle(irr);
}

Map::Map(ushort column, ushort line, float blockSize, const std::shared_ptr<Irrlicht> &irr) : _blocksize(blockSize), _line(line), _column(column),
    _procedural(column, line)
{
    auto sol = irr->createTexture("asset/map/sol.png");
    irr->getSceneManager()->addLightSceneNode(nullptr, irr::core::vector3df(-10, 25, -10));

    for (int nb = 0; nb < line * column; nb++)
        _floorNodes.push_back(createFloorBlock((float)(nb % column), 0, (float)(nb / column), irr->getSceneManager(), sol));
}

irr::scene::IMeshSceneNode *Map::createFloorBlock(float x, float y, float z, irr::scene::ISceneManager *m, irr::video::ITexture *tex) const
{
    auto block = m->addCubeSceneNode(_blocksize + (float)0.3, nullptr, -1, irr::core::vector3df(x * _blocksize, y * _blocksize, z * _blocksize));

    block->setMaterialTexture(0, tex);
    block->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    block->setRotation({0, 90, 0});
    return block;
}

void Map::add(const std::shared_ptr<Bomb> &bomb)
{
    _bombs.push_back(bomb);
}

void Map::createExternalWall(const std::shared_ptr<Irrlicht> &irr, const irr::core::vector3df &vector, std::pair<int, int> pos,
    irr::video::ITexture *texture
)
{
    irr::scene::IMeshSceneNode *node_under = irr->getSceneManager()->addCubeSceneNode(_blocksize, nullptr, -1, vector);

    node_under->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node_under->addShadowVolumeSceneNode();
    add(std::make_shared<Block>(nullptr, node_under, texture, false), pos.first, pos.second);
}

void Map::createExternalWalls(const std::shared_ptr<Irrlicht> &irr)
{
    auto limit = irr->createTexture("asset/map/limit.png");
    // BOTTOM
    irr::core::vector3df start = {0, 0, -_blocksize};
    for (int i = -1; (float)i < _column + 1; i++)
    {
        createExternalWall(irr, irr::core::vector3df((float)i * _blocksize, start.Y, start.Z), std::make_pair(i, -1), limit);
        createExternalWall(irr, irr::core::vector3df((float)i * _blocksize, _blocksize, start.Z), std::make_pair(i, -1), limit);
    }

    // LEFT
    start = {-_blocksize, 0, 0};
    for (int i = 0; (float)i < _line; i++)
    {
        createExternalWall(irr, irr::core::vector3df(start.X, start.Y, (float)i * _blocksize), std::make_pair(-1, i), limit);
        createExternalWall(irr, irr::core::vector3df(start.X, _blocksize, (float)i * _blocksize), std::make_pair(-1, i), limit);
    }

    // RIGHT
    start = getMapCorner(Corner::BotRight);
    start.X += _blocksize;
    for (int i = 0; (float)i < _line; i++)
    {
        createExternalWall(irr, irr::core::vector3df(start.X, start.Y, (float)i * _blocksize), std::pair<int, int>((int)_column, i), limit);
        createExternalWall(irr, irr::core::vector3df(start.X, _blocksize, (float)i * _blocksize), std::pair<int, int>((int)_column, i), limit);
    }

    // TOP
    start = getMapCorner(Corner::TopLeft);
    start.Z += _blocksize;
    for (int i = -1; (float)i < _column + 1; i++)
    {
        createExternalWall(irr, irr::core::vector3df((float)i * _blocksize, start.Y, start.Z), std::pair<int, int>(i, (int)_line), limit);
        createExternalWall(irr, irr::core::vector3df((float)i * _blocksize, _blocksize, start.Z), std::pair<int, int>(i, (int)_line), limit);
    }
}

void Map::placeObstacle(const std::shared_ptr<Irrlicht> &irr)
{
    std::shared_ptr<Block> cpy = nullptr;
    auto texBreak = irr->createTexture("asset/map/block_breakable.png");
    auto texUnbreak = irr->createTexture("asset/map/block_unbreakable.png");

    for (int i = 0; i < (int)(_line * _column); ++i)
    {
        if (!_procedural[i])
            continue;
        irr::core::vector3df tmp = _floorNodes[i]->getPosition();
        tmp.Y += (float)(_blocksize * 0.9);
        irr::scene::IMeshSceneNode *node_under = irr->getSceneManager()->addCubeSceneNode(_blocksize, nullptr, -1, tmp);
        node_under->setScale({(float)1, (float)0.9, (float)1});
        node_under->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node_under->addShadowVolumeSceneNode();
        if (_procedural[i] > 2)
            placePowerUp(irr, i, _procedural[i], tmp);
        cpy = std::make_shared<Block>(nullptr, node_under, _procedural[i] != 1 ? texBreak : texUnbreak, _procedural[i] != 1);
        _entityMap[std::pair<int, int>((int)std::fmod(i, _column), (int)std::floor((float)i / _column))] = cpy;
    }
}

void Map::add(const std::shared_ptr<Block> &entity, int posX, int posY)
{
    _entityMap[std::pair<int, int>(posX, posY)] = entity;
}

irr::core::vector3df Map::getMapCorner(Map::Corner corner)
{
    return _mapCorner[corner];
}

void Map::placePowerUp(const std::shared_ptr<Irrlicht> &irr, int idx, int type, const irr::core::vector3df &pos)
{
    irr::scene::IMeshSceneNode *node_under = irr->getSceneManager()->addCubeSceneNode(_blocksize / 2, nullptr, -1, pos);
    node_under->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    std::shared_ptr<PowerUpBlock> cpy = std::make_shared<PowerUpBlock>(nullptr, node_under, irr->createTexture(
        type == 3 ? "asset/map/powerUpBomberPower.png" : type == 4 ? "asset/map/powerUpBombNumber.png" :
            type == 5 ? "asset/map/powerUpSpeed.png" : type == 6 ? "asset/map/powerUpWallPass.png" : ""), type);
    _powerUpMap[std::pair<int, int>((int)std::fmod(idx, _column), (int)std::floor((float)idx / _column))] = cpy;
}

std::map<std::pair<int, int>, std::shared_ptr<PowerUpBlock>> &Map::getPowerUpMap()
{
    return _powerUpMap;
}

void Map::reset(std::shared_ptr<Irrlicht> &_irr)
{
    _entityMap.clear();
    _floorNodes.clear();
    _powerUpMap.clear();
}
