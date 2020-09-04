//
// Created by guillaumeg on 31/05/2020.
//

#ifndef INDIE_STUDIO_ISCENE_HPP
#define INDIE_STUDIO_ISCENE_HPP

#include <engine/Irrlicht.hpp>
#include <utils/Enum.hpp>

class IScene
{
    public:
    virtual ~IScene() = default;

    virtual Enum::Scene update() = 0;

    virtual void setup() = 0;

    virtual void unsetup() = 0;

    virtual bool adjustCamera() = 0;

    virtual Enum::Scene getScene() = 0;

    [[nodiscard]] virtual const std::pair<irr::core::vector3df, irr::core::vector3df> &getCameraInfo() const = 0;
};

#endif //INDIE_STUDIO_ISCENE_HPP
