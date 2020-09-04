//
// Created by seres on 28/05/2020.
//

#ifndef INDIE_STUDIO_BOMB_HPP
#define INDIE_STUDIO_BOMB_HPP

#include <vector>
#include <entity/AEntity.hpp>
#include <clock/Clock.hpp>
#include <irrlicht/IMeshSceneNode.h>

class Bomb : public AEntity
{
    public:

    public:
    Bomb(irr::scene::IMesh *mesh, irr::scene::IMeshSceneNode *node, irr::video::ITexture *t, int &reload, int power) :
        AEntity(mesh, node, t), _reload(reload), _power(power), _pos(node->getPosition())
    {
    };

    Bomb(Bomb &&) = default;

    void addExplosionCylender(const std::pair<std::shared_ptr<AEntity>, irr::scene::ISceneNodeAnimator *> &cylender)
    {
        _explosionCylender.push_back(cylender);
    }

    bool destroy();

    int &getReload() { return _reload; };

    [[nodiscard]] const std::vector<std::pair<std::shared_ptr<AEntity>, irr::scene::ISceneNodeAnimator *>> &getExplosionCylender() const;
    [[nodiscard]] int getPower() const;
    [[nodiscard]] const irr::core::vector3df &getPosition() const { return _pos; }


private:
    std::vector<std::pair<std::shared_ptr<AEntity>, irr::scene::ISceneNodeAnimator *>> _explosionCylender;
    Clock timer;
    int &_reload;
    int _power;
    irr::core::vector3df _pos;
};

#endif //INDIE_STUDIO_BOMB_HPP
