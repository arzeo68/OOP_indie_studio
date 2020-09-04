//
// Created by Laurent on 6/4/2020.
//

#ifndef INDIE_STUDIO_MUSIC_HPP
#define INDIE_STUDIO_MUSIC_HPP

#include <string>
#include <engine/ASound.hpp>
#include <SFML/Audio/Music.hpp>

class Music : public ASound
{
    public:
    explicit Music(const std::string &path);

    void play() override;

    void pause() override;

    sf::SoundSource::Status getStatus() override;

    void stop() override;

    void setVolume(int volume) noexcept override;

    private:
    sf::Music _music;
};

#endif //INDIE_STUDIO_MUSIC_HPP
