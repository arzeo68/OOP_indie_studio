//
// Created by seres on 18/05/2020.
//

#ifndef INDIE_STUDIO_IRRLICHTDELETER_HPP
#define INDIE_STUDIO_IRRLICHTDELETER_HPP

template<typename T>
struct IrrlichtDeleter
{
    void operator()(T *p) const
    {
        p->drop();
    };
};

#endif //INDIE_STUDIO_IRRLICHTDELETER_HPP
