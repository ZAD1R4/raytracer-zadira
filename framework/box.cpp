#include "box.hpp"

Box::Box(glm::vec3 const& min, glm::vec3 const& max, Color const& color, std::string const& name)
    : Shape{name, color}
, min_{min}
, max_{max}
{
}

float Box::area() const
{
    glm::vec3 size = max_ - min_;
    return 2.0f * (size.x * size.y + size.y * size.z + size.z * size.x);
}

float Box::volume() const
{
    glm::vec3 size = max_ - min_;
    return size.x * size.y * size.z;
}

std::ostream& Box::print(std::ostream& os) const
{
    Shape::print(os);
    os << ", Box: min = (" << min_.x << ", " << min_.y << ", " << min_.z << ")"
       << ", max = (" << max_.x << ", " << max_.y << ", " << max_.z << ")";
    return os;
}