//
// Created by alexis on 28/05/2020.
//

#ifndef INDIE_STUDIO_BLOCK_HPP
#define INDIE_STUDIO_BLOCK_HPP

#include <entity/AEntity.hpp>

class Block : public AEntity
{
    public:
    Block(irr::scene::IMesh *mesh, irr::scene::IMeshSceneNode *node, irr::video::ITexture *texture, bool isBreakable) : AEntity(mesh, node, texture),
        _isBreakable(isBreakable)
    {
    };

    Block(Block &&) = default;

    bool die(const std::shared_ptr<Irrlicht> &_irr, int timeMs) override;
    bool isBreakable() override;

    private:
    bool _isBreakable = false;
};

#endif //INDIE_STUDIO_BLOCK_HPP
