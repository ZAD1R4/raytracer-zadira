#include "shape.hpp"

Shape::Shape(std::string const& name, std::shared_ptr<Material> const& material)
    : name_{name}
, material_{material}
{
    std::cout << "Shape::Shape() - constructing \"" << name_ << "\"\n";
}

Shape::~Shape()
{
    std::cout << "Shape::~Shape() - destructing \"" << name_ << "\"\n";
}

std::ostream& Shape::print(std::ostream& os) const
{
    os << "Shape \"" << name_ << "\"";
    if (material_) {
        os << ", " << *material_;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, Shape const& s)
{
    s.print(os);
    return os;
}