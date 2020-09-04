//
// Created by arzeo on 6/3/2020.
//

#include <entity/block/powerUpBlock/PowerUpBlock.hpp>

PowerUpBlock::PowerUpBlock(irr::scene::IMesh *mesh, irr::scene::IMeshSceneNode *node, irr::video::ITexture *texture, int type) : Block(mesh, node,
    texture, false), _type(type)
{
}

int PowerUpBlock::getType() const
{
    return _type;
}
