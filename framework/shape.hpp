#pragma once

#include "color.hpp"
#include <string>

class Shape
{
public:
    Shape(std::string const& name, Color const& color);
    virtual ~Shape() = default;

    virtual float area() const = 0;
    virtual float volume() const = 0;

protected:
    std::string name_;
    Color color_;
};