#include "sphere.hpp"
#include <glm/gtc/constants.hpp>

Sphere::Sphere(glm::vec3 const& center, float radius, Color const& color, std::string const& name)
    : Shape{name, color}
, center_{center}
, radius_{radius}
{
}

float Sphere::area() const
{
    return 4.0f * glm::pi<float>() * radius_ * radius_;
}

float Sphere::volume() const
{
    return (4.0f / 3.0f) * glm::pi<float>() * radius_ * radius_ * radius_;
}