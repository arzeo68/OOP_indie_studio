//
// Created by guillaumeg on 12/06/2020.
//

#ifndef INDIE_STUDIO_SAVE_HPP
#define INDIE_STUDIO_SAVE_HPP

#include <map>
#include <utility>
#include "engine/Tracks.hpp"
#include "utils/File.hpp"
class Save {
public:
    explicit Save(Tracks &options);
    ~Save();

    void addScore(int score) noexcept;
    void save() noexcept;

    [[nodiscard]] Scores &getScores() noexcept { return _scores; }
private:
    Tracks &_tracks;
    File _config;
    Scores _scores;
};

#endif //INDIE_STUDIO_SAVE_HPP
