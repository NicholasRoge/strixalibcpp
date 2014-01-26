#include <Strixa/Graphics/Dimensions.h>

using Strixa::Graphics::Dimensions;


Dimensions::Dimensions(long width,long height,long depth)
: width(raw[0]),height(raw[1]),depth(raw[2])
{
    this->depth = depth;
    this->height = height;
    this->width = width;
}

Dimensions::Dimensions(const Dimensions& copy)
: width(raw[0]),height(raw[1]),depth(raw[2])
{
    *this = copy;
}

bool Dimensions::operator==(const Dimensions& comp) const
{
    return this->width == comp.width && this->height == comp.height && this->depth == comp.depth;
}

bool Dimensions::operator!=(const Dimensions& comp) const
{
    return !(*this == comp);
}

Dimensions& Dimensions::operator=(const Dimensions& copy)
{
    this->width = copy.width;
    this->height = copy.height;
    this->depth = copy.depth;

    return *this;
}