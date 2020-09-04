//
// Created by seres on 28/05/2020.
//

#include <entity/bomb/Bomb.hpp>

bool Bomb::destroy()
{
    return timer.getElapsedTimeSinceLastReset() >= 2;
}

const std::vector<std::pair<std::shared_ptr<AEntity>, irr::scene::ISceneNodeAnimator *>> &Bomb::getExplosionCylender() const
{
    return _explosionCylender;
}

int Bomb::getPower() const
{
    return _power;
}

