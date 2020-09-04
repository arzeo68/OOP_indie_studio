//
// Created by guillaumeg on 02/06/2020.
//

#ifndef INDIE_STUDIO_BACKGROUND_HPP
#define INDIE_STUDIO_BACKGROUND_HPP

#include <clock/Clock.hpp>
#include <memory>
#include <engine/Irrlicht.hpp>

class Background
{
    public:
    Background(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock);

    Background(Background &&) = default;

    ~Background() = default;

    void update();

    private:
    std::shared_ptr<Irrlicht> &_irr;
    std::shared_ptr<Clock> &_clock;
    irr::video::ITexture *_texture;
};

#endif //INDIE_STUDIO_BACKGROUND_HPP
