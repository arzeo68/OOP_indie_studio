//
// Created by alexis on 27/05/2020.
//

#ifndef INDIE_STUDIO_MAP_HPP
#define INDIE_STUDIO_MAP_HPP

#include <irrlicht/ISceneManager.h>
#include <vector>
#include <entity/bomb/Bomb.hpp>
#include <entity/player/Player.hpp>
#include <map>
#include <map/ProceduralMap.hpp>
#include <entity/block/powerUpBlock/PowerUpBlock.hpp>

class Map
{
    public:
    enum Corner
    {
        TopLeft,
        TopRight,
        BotLeft,
        BotRight,
    };

    explicit Map(const std::shared_ptr<Irrlicht> &irr);

    Map(ushort column, ushort line, float blockSize, const std::shared_ptr<Irrlicht> &irr);

    Map(Map &&map) = default;

    void reset(std::shared_ptr<Irrlicht> &_irr);

    /*------------------------GETTER----------------------------*/
    [[nodiscard]] const std::vector<irr::scene::IMeshSceneNode *> &getFloorNodeVector() const
    {
        return _floorNodes;
    }

    [[nodiscard]] irr::core::vector3df getMapCorner(Corner corner);

    [[nodiscard]] irr::core::vector3df getMapCenter() const
    {
        return {(float)((_column / 2) - 0.5) * _blocksize, 0, (float)((_line / 2) - 0.5) * _blocksize};
    }

    [[nodiscard]] std::map<std::pair<int, int>, std::shared_ptr<Block>> &getEntities()
    {
        return _entityMap;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Bomb>> &getBombs()
    {
        return _bombs;
    }

    [[nodiscard]] float getBlocksize() const
    {
        return _blocksize;
    }

    [[nodiscard]] float getLine() const
    {
        return _line;
    }

    [[nodiscard]] float getColumn() const
    {
        return _column;
    }

    [[nodiscard]] ProceduralMap &getProceduralMap()
    {
        return _procedural;
    }

    [[nodiscard]] std::pair<irr::core::vector3df, irr::core::vector3df> getCameraPosition() const
    {
        irr::core::vector3df center = getMapCenter();
        irr::core::vector3df pos = {center.X, (float)(center.Y + center.Z * 2.2), (float)(center.Z - center.Z * 1.2)};
        return std::make_pair(pos, center);
    }

    std::map<std::pair<int, int>, std::shared_ptr<PowerUpBlock>> &getPowerUpMap();

    void add(const std::shared_ptr<Block> &entity, int posX, int posY);

    void add(const std::shared_ptr<Bomb> &bomb);

    void placePowerUp(const std::shared_ptr<Irrlicht> &irr, int idx, int type, const irr::core::vector3df &pos);

private:
    float _line = 0;
    float _column = 0;
    float _blocksize;
    ProceduralMap _procedural;
    std::map<Corner, irr::core::vector3df> _mapCorner = {{std::make_pair<Corner, irr::core::vector3df>(Corner::BotLeft, {0, 0, 0})},
        {std::make_pair<Corner, irr::core::vector3df>(Corner::BotRight, {(_column - 1) * _blocksize, 0, 0})},
        {std::make_pair<Corner, irr::core::vector3df>(Corner::TopLeft, {0, 0, (_line - 1) * _blocksize,})},
        {std::make_pair<Corner, irr::core::vector3df>(Corner::TopRight, {(_column - 1) * _blocksize, 0, (_line - 1) * _blocksize})}};
    std::map<std::pair<int, int>, std::shared_ptr<Block>> _entityMap;
    std::map<std::pair<int, int>, std::shared_ptr<PowerUpBlock>> _powerUpMap;
    std::vector<irr::scene::IMeshSceneNode *> _floorNodes;
    std::vector<std::shared_ptr<Bomb>> _bombs;

    void createExternalWall(const std::shared_ptr<Irrlicht> &irr, const irr::core::vector3df &vector, std::pair<int, int> pos,
        irr::video::ITexture *texture
    );

    void createExternalWalls(const std::shared_ptr<Irrlicht> &irr);

    irr::scene::IMeshSceneNode *createFloorBlock(float x, float y, float z, irr::scene::ISceneManager *m, irr::video::ITexture *tex) const;

    void placeObstacle(const std::shared_ptr<Irrlicht> &irr);
};

#endif //INDIE_STUDIO_MAP_HPP
