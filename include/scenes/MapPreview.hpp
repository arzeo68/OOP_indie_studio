//
// Created by guillaumeg on 07/06/2020.
//

#ifndef INDIE_STUDIO_MAPPREVIEW_HPP
#define INDIE_STUDIO_MAPPREVIEW_HPP

#include "scenes/AMenu.hpp"
#include "scenes/Game.hpp"

class MapPreview : public AMenu
{
    public:
    MapPreview(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks, Game *game);

    MapPreview(MapPreview &&) = default;

    Enum::Scene update() final;

    void setup() final;

    void unsetup() final;

    Enum::Scene getScene() override;

    bool adjustCamera() final;

    private:
    std::shared_ptr<Map> &_map;
};

#endif //INDIE_STUDIO_MAPPREVIEW_HPP
