//
// Created by arzeo on 6/12/2020.
//

#ifndef INDIE_STUDIO_LADDER_HPP
#define INDIE_STUDIO_LADDER_HPP

#include "AMenu.hpp"
#include "utils/File.hpp"

class Ladder : public AMenu
{
    public:
    Ladder(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks,
            Scores &scores);

    Enum::Scene update() override;

    Enum::Scene getScene() override;

    void setup() override;

    void unsetup() override;

private:
    Scores &_scores;
    static int findLargestWord(std::multimap<int, std::wstring> multimap);
};

#endif //INDIE_STUDIO_LADDER_HPP
