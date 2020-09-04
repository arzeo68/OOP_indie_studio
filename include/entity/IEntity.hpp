//
// Created by alexis on 26/05/2020.
//

#ifndef INDIE_STUDIO_IENTITY_HPP
#define INDIE_STUDIO_IENTITY_HPP

#include <memory>
#include <engine/Irrlicht.hpp>
#include <irrlicht/vector3d.h>
#include <irrlicht/IAnimatedMeshSceneNode.h>

class IEntity
{
    public:
    virtual ~IEntity() = default;

    virtual irr::scene::IAnimatedMeshSceneNode *getNode() = 0;

    virtual void setRotation(const irr::core::vector3df &rotation) = 0;

    virtual bool setFrameLoop(irr::s32 begin, irr::s32 end) = 0;

    virtual void setAnimationSpeed(irr::f32 framesPerSecond) = 0;

    virtual void setPosition(const irr::core::vector3df &newpos) = 0;

    virtual void move(const irr::core::vector3df &newpos) = 0;

    virtual bool die(const std::shared_ptr<Irrlicht> &_irr, int timeMs) = 0;

    virtual bool isBreakable() = 0;

    [[nodiscard]] virtual bool isAlive() const = 0;

    virtual void setAlive(bool alive) = 0;
};

#endif //INDIE_STUDIO_IENTITY_HPP
