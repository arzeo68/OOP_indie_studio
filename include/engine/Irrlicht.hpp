//
// Created by seres on 04/05/2020.
//

#ifndef IRRLICHT_IRRLICHT_HPP
#define IRRLICHT_IRRLICHT_HPP

#include <memory>
#include <irrlicht/irrlicht.h>
#include <utils/IrrlichtDeleter.hpp>
#include <engine/UserInput.hpp>
#include <array>
#include <string>
#include <vector>

struct nop
{
    template<typename T>
    void operator()(T const &) const noexcept
    {
    }
};

class Irrlicht
{
public:
    Irrlicht(int width, int heigth);

    Irrlicht(Irrlicht &&) = default;

    ~Irrlicht();

    [[nodiscard]] const std::unique_ptr<irr::IrrlichtDevice, IrrlichtDeleter<irr::IrrlichtDevice>> &getDevice() const;

    [[nodiscard]] const std::unique_ptr<irr::gui::IGUIEnvironment, nop> &getGuiEnv() const;

    [[nodiscard]] const std::unique_ptr<irr::video::IVideoDriver, nop> &getVDriver() const;

    void setWindowTitle(const wchar_t *title);

    irr::scene::ICameraSceneNode *addCamera(const irr::core::vector3df &placement, const irr::core::vector3df &lookat);

    irr::gui::IGUIStaticText *createText(const wchar_t *text, irr::s32 x1, irr::s32 y1, irr::s32 x2, irr::s32 y2);

    irr::scene::IMesh *createMesh(const std::string &filePath);

    irr::scene::IAnimatedMesh *createAnimatedMesh(const std::string &filePath);

    irr::scene::IMeshSceneNode *createNodeFromMesh(irr::scene::IMesh *mesh);

    irr::scene::IAnimatedMeshSceneNode *createNodeFromAnimatedMesh(irr::scene::IAnimatedMesh *mesh);

    irr::video::ITexture *createTexture(const std::string &texturePath);

    irr::gui::IGUIButton *createButton(std::array<int, 4> arr, irr::s32 id, const std::string &imagePath);

    irr::gui::IGUIEditBox *createTextBox(std::array<int, 4> arr, irr::s32 id);

    irr::scene::ISceneManager *getSceneManager();

    std::vector<std::string> &getPodiumPlacement();

    [[nodiscard]] const std::unique_ptr<UserInput, nop> &getInput() const;

    [[nodiscard]] irr::gui::IGUIInOutFader *getFader() const
    {
        return _fader;
    }

    void setFader(irr::gui::IGUIInOutFader *fader)
    {
        _fader = fader;
    }

    [[nodiscard]] irr::gui::IGUIFont *getLoadedFont() const noexcept
    {
        return _font;
    }

    [[nodiscard]] static irr::core::recti getCenter(int elementHeight, int elementWidth)
    {
        return (irr::core::recti(1280 / 2 - elementWidth / 2, 720 / 2 - elementHeight / 2, 1280 / 2 + elementWidth / 2, 720 / 2 + elementHeight / 2));
    }

    private:
    std::unique_ptr<UserInput, nop> _input;
    irr::video::E_DRIVER_TYPE _driverType;
    std::unique_ptr<irr::IrrlichtDevice, IrrlichtDeleter<irr::IrrlichtDevice>> _device;
    std::unique_ptr<irr::gui::IGUIEnvironment, nop> _guiEnv;
    std::unique_ptr<irr::video::IVideoDriver, nop> _vDriver;
    irr::scene::ISceneManager *_sManager;
    irr::gui::IGUIInOutFader *_fader;
    std::vector<std::string> podiumPlacement;
    bool _secondPlayer = false;
    public:
    bool isSecondPlayer() const;

    void setSecondPlayer(bool secondPlayer);

    irr::gui::IGUIFont *_font;
};

#endif //IRRLICHT_IRRLICHT_HPP