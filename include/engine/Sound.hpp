//
// Created by guillaumeg on 04/06/2020.
//

#ifndef INDIE_STUDIO_NOISE_HPP
#define INDIE_STUDIO_NOISE_HPP

#include <SFML/Audio.hpp>
#include "ASound.hpp"

class Sound : public ASound
{
    public:
    explicit Sound(const std::string &path);

    ~Sound() = default;

    sf::SoundSource::Status getStatus() override;

    Sound(const Sound &) = default;

    Sound &operator=(const Sound &) = default;

    void play() noexcept override;

    void pause() noexcept override;

    void stop() noexcept override;

    void setVolume(int volume) noexcept override;

    private:
    sf::SoundBuffer _buffer;
    sf::Sound _sound;
};

#endif //INDIE_STUDIO_SOUND_HPP
