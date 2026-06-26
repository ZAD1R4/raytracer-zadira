#include "shape.hpp"

Shape::Shape(std::string const& name, Color const& color)
    : name_{name}
, color_{color}
{
}

std::ostream& Shape::print(std::ostream& os) const
{
    os << "Shape \"" << name_ << "\", color: " << color_;
    return os;
}

std::ostream& operator<<(std::ostream& os, Shape const& s)
{
    s.print(os);
    return os;
}