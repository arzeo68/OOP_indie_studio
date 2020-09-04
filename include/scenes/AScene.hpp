//
// Created by guillaumeg on 02/06/2020.
//

#ifndef INDIE_STUDIO_ASCENE_HPP
#define INDIE_STUDIO_ASCENE_HPP

#include <utility>
#include <scenes/IScene.hpp>
#include <irrlicht/ISceneManager.h>
#include <engine/Irrlicht.hpp>
#include <clock/Clock.hpp>
#include <engine/Tracks.hpp>

#define IS_BETWEEN(indice, value, goal) (goal - indice <= value && goal + indice >= value)

class AScene : public IScene
{
    public:
    AScene(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks,
        std::pair<irr::core::vector3df, irr::core::vector3df> vectors) :
        _irr(irr), _clock(clock), _camera(camera), _tracks(tracks), _cameraPositions(std::move(vectors))
    {
    }

    AScene(AScene &&) = default;

    void setup() override
    {
    }

    void unsetup() override
    {
    }

    bool adjustCamera() override
    {
        static auto coeff = (float)0.6;
        static auto indice = (float)0.3;
        auto infos = _cameraPositions;
        auto pos = _camera->getPosition();
        auto target = _camera->getTarget();

        if (roundVector(pos) == roundVector(infos.first) && roundVector(target) == roundVector(infos.second))
            return true;
        pos.X += IS_BETWEEN(indice, pos.X, infos.first.X) ? 0 : pos.X > infos.first.X ? -coeff : coeff;
        pos.Y += IS_BETWEEN(indice, pos.Y, infos.first.Y) ? 0 : pos.Y > infos.first.Y ? -coeff : coeff;
        pos.Z += IS_BETWEEN(indice, pos.Z, infos.first.Z) ? 0 : pos.Z > infos.first.Z ? -coeff : coeff;
        target.X += IS_BETWEEN(indice, target.X, infos.second.X) ? 0 : target.X > infos.second.X ? -coeff : coeff;
        target.Y += IS_BETWEEN(indice, target.Y, infos.second.Y) ? 0 : target.Y > infos.second.Y ? -coeff : coeff;
        target.Z += IS_BETWEEN(indice, target.Z, infos.second.Z) ? 0 : target.Z > infos.second.Z ? -coeff : coeff;
        _camera->setTarget(target);
        _camera->setPosition(pos);
        return false;
    }

    [[nodiscard]] const std::pair<irr::core::vector3df, irr::core::vector3df> &getCameraInfo() const override { return _cameraPositions; }

protected:
    std::shared_ptr<Irrlicht> &_irr;
    std::shared_ptr<Clock> &_clock;
    irr::scene::ICameraSceneNode *&_camera;
    Tracks &_tracks;
    std::pair<irr::core::vector3df, irr::core::vector3df> _cameraPositions;

private:
    static irr::core::vector3df roundVector(const irr::core::vector3df& val)
    {
        return {std::round(val.X), std::round(val.Y), std::round(val.Z)};
    };
};

#endif //INDIE_STUDIO_ASCENE_HPP
