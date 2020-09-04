//
// Created by guillaumeg on 09/06/2020.
//

#ifndef INDIE_STUDIO_LOGIN_HPP
#define INDIE_STUDIO_LOGIN_HPP

#include "AMenu.hpp"

class Login : public AMenu
{
    public:
    Login(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks);

    Login(Login &&) = default;

    Enum::Scene getScene() override;

    Enum::Scene update() override;

    void setup() override;

    void unsetup() override;

    private:
    irr::gui::IGUIEditBox *_textBox;
    irr::gui::IGUIStaticText *_error;
};

#define BACKGROUND_MUSIC "asset/menu/bomberman.ogg"

#endif //INDIE_STUDIO_LOGIN_HPP
