//
// Created by alexis on 28/05/2020.
//

#include <entity/block/Block.hpp>

bool Block::die(const std::shared_ptr<Irrlicht> &_irr, int timeMs)
{
    _node->setMaterialTexture(0, _irr->createTexture("asset/map/block_exploded.png"));
    return AEntity::die(_irr, timeMs);
}

bool Block::isBreakable()
{
    return _isBreakable;
}
