#pragma once

#include "color.hpp"
#include <string>
#include <iostream>

class Shape
{
public:
    Shape(std::string const& name, Color const& color);
    virtual ~Shape() = default;

    virtual float area() const = 0;
    virtual float volume() const = 0;

    virtual std::ostream& print(std::ostream& os) const;

protected:
    std::string name_;
    Color color_;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);