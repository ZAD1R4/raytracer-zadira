#define GLM_ENABLE_EXPERIMENTAL
#include "sphere.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/intersect.hpp>

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

std::ostream& Sphere::print(std::ostream& os) const
{
    Shape::print(os);
    os << ", Sphere: center = (" << center_.x << ", " << center_.y << ", " << center_.z << ")"
       << ", radius = " << radius_;
    return os;
}

HitPoint Sphere::intersect(Ray const& ray) const
{
    HitPoint result;

    float distance = 0.0f;
    bool did_hit = glm::intersectRaySphere(
        ray.origin, ray.direction,
        center_,
        radius_ * radius_,
        distance);

    result.hit = did_hit;

    if (did_hit) {
        result.distance = distance;
        result.name = name_;
        result.color = color_;
        result.intersection_point = ray.origin + distance * ray.direction;
        result.direction = ray.direction;
    }

    return result;
}