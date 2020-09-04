//
// Created by seres on 05/05/2020.
//

#ifndef IRRLICHT_IRRLICHEXCEPTION_HPP
#define IRRLICHT_IRRLICHEXCEPTION_HPP

#include <cstring>
#include <exception>
#include <utility>
#include <string>

class IrrlichtException : public std::exception
{
    public:
    explicit IrrlichtException(std::string message = "") : _message(std::move(message))
    {
    };

    ~IrrlichtException() override = default;

    [[nodiscard]] const char *what() const noexcept override
    {
        return _message.c_str();
    }

    private:
    std::string _message;
};

#endif //IRRLICHT_IRRLICHEXCEPTION_HPP
