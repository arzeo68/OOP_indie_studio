//
// Created by alexis on 26/05/2020.
//

#ifndef INDIE_STUDIO_AENTITY_HPP
#define INDIE_STUDIO_AENTITY_HPP

#include <irrlicht/IAnimatedMeshSceneNode.h>
#include <irrlicht/IMeshSceneNode.h>
#include <string>
#include <entity/IEntity.hpp>
#include <iostream>

class AEntity : public IEntity
{
    public:
    AEntity(irr::scene::IAnimatedMesh *mesh, irr::scene::IAnimatedMeshSceneNode *node, irr::video::ITexture *texture) : _mesh(mesh), _node(node),
        _texture(texture)
    {
        _node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        _node->setMaterialTexture(0, _texture);
        _node->setAnimationSpeed(30);
    }

    AEntity(irr::scene::IMesh *mesh, irr::scene::IMeshSceneNode *node, irr::video::ITexture *texture) : _mesh(mesh),
        _node(reinterpret_cast<irr::scene::IAnimatedMeshSceneNode *>(node)), _texture(texture)
    {
        _node->setMaterialTexture(0, _texture);
    }

    AEntity(AEntity &&) = default;

    ~AEntity() override = default;

    irr::scene::IAnimatedMeshSceneNode *getNode() override { return _node; }

    void setRotation(const irr::core::vector3df &rotation) override
    {
        return _node->setRotation(irr::core::vector3df(rotation));
    }

    bool setFrameLoop(irr::s32 begin, irr::s32 end) override
    {
        return _node->setFrameLoop(begin, end);
    }

    void setAnimationSpeed(irr::f32 framesPerSecond) override
    {
        return _node->setAnimationSpeed(framesPerSecond);
    }

    void setPosition(const irr::core::vector3df &newpos) override
    {
        return _node->setPosition(newpos);
    }

    void move(const irr::core::vector3df &newpos) override
    {
        irr::core::vector3df old = _node->getPosition();
        _node->setPosition(old + newpos);
    }

    bool die(const std::shared_ptr<Irrlicht> &_irr, int timeMs) override
    {
        auto animator = _irr->getSceneManager()->createDeleteAnimator(timeMs);
        _node->addAnimator(animator);
        animator->drop();
        return true;
    }

    bool isBreakable() override { return false; }

    [[nodiscard]] bool isAlive() const override { return _isAlive; }

    void setAlive(bool alive) override { _isAlive = alive; }

protected:
    irr::scene::IMesh *_mesh;
    irr::scene::IAnimatedMeshSceneNode *_node;
    irr::video::ITexture *_texture;
    bool _isAlive = true;
};

#endif //INDIE_STUDIO_AENTITY_HPP
