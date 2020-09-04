//
// Created by alexis on 27/05/2020.
//

#ifndef INDIE_STUDIO_USERINPUT_HPP
#define INDIE_STUDIO_USERINPUT_HPP

#include <irrlicht/IEventReceiver.h>

class UserInput : public irr::IEventReceiver
{
    public:
    UserInput() = default;

    UserInput(UserInput &&) = default;

    bool OnEvent(const irr::SEvent &event) override
    {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        if (event.EventType == irr::EET_GUI_EVENT && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
            _idButtonPressed = event.GUIEvent.Caller->getID();
        return false;
    }

    [[nodiscard]] virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

    irr::s32 exportIDButtonPressed()
    {
        irr::s32 id = _idButtonPressed;

        _idButtonPressed = -1;
        return id;
    }

    private:
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT] = {false};
    irr::s32 _idButtonPressed = -1;
};

#endif //INDIE_STUDIO_USERINPUT_HPP
