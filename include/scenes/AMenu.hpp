//
// Created by arzeo on 6/5/2020.
//

#ifndef INDIE_STUDIO_AMENU_HPP
#define INDIE_STUDIO_AMENU_HPP

#include <utility>
#include "AScene.hpp"

class AMenu : public AScene {
public:
    enum ButtonID
    {
        Play = 1,
        Options,
        BackToMenu,
        PlusMusic,
        MinusMusic,
        PlusSound,
        MinusSound,
        SwitchKeyBoard,
        ReloadMap,
        UserName,
        Advance,
        Join,
        Ladder
    };

    AMenu(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks,
        std::pair<irr::core::vector3df, irr::core::vector3df> vectors = {{-55.0, 19.2, -7.8}, {-55.0, 0, 6.0}}) :
        AScene(irr, clock, camera, tracks, std::move(vectors))
    {
    };

    void unsetup() override
    {
        _buttons.clear();
        _irr->getGuiEnv()->clear();
    };

protected:
    std::vector<irr::gui::IGUIButton *> _buttons;
    std::vector<irr::gui::IGUIStaticText *> _texts;
};

#endif //INDIE_STUDIO_AMENU_HPP
