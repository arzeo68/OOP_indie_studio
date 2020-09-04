//
// Created by arzeo on 6/3/2020.
//

#ifndef INDIE_STUDIO_POWERUPBLOCK_HPP
#define INDIE_STUDIO_POWERUPBLOCK_HPP

#include <entity/block/Block.hpp>

class PowerUpBlock : public Block
{
    public:
    PowerUpBlock(irr::scene::IMesh *mesh, irr::scene::IMeshSceneNode *node, irr::video::ITexture *texture, int type);

    PowerUpBlock(PowerUpBlock &&) = default;

    [[nodiscard]] int getType() const;

private:
    int _type;
};

#endif //INDIE_STUDIO_POWERUPBLOCK_HPP
