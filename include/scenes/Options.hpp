//
// Created by guillaumeg on 04/06/2020.
//

#ifndef INDIE_STUDIO_OPTIONS_HPP
#define INDIE_STUDIO_OPTIONS_HPP

#include "AMenu.hpp"
#include "utils/Save.hpp"

class Options : public AMenu {
public:
    Options(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock,
            irr::scene::ICameraSceneNode *&camera, Tracks &tracks, Save &save);

    Enum::Scene update() override;

    Enum::Scene getScene() override;

    void setup() override;

    void unsetup() override;

private:
    irr::video::ITexture *_textures[2];
    irr::video::ITexture *_keyboard[2];
    Save &_save;
};

#endif //INDIE_STUDIO_OPTIONS_HPP
