//
// Created by guillaumeg on 29/05/2020.
//

#ifndef MAPPROCESS_MAPEXCEPTION_HPP
#define MAPPROCESS_MAPEXCEPTION_HPP

#include <exception>
#include <string>
#include <sstream>

class MapException : public std::exception
{
    public:
    explicit MapException(const std::string &file, int line, const std::string &msg)
    {
        std::stringstream stream;

        stream << "[ " << file << " : " << line << " ] : " << msg << std::endl;
        _error = stream.str();
    }

    ~MapException() override = default;

    MapException(const MapException &) = default;

    MapException &operator=(const MapException &) = default;

    [[nodiscard]] const char *what() const noexcept override
    {
        return _error.c_str();
    }

    protected:
    std::string _error;
};

#endif //MAPPROCESS_MAPEXCEPTION_HPP
