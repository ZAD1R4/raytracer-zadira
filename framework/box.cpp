#include "box.hpp"
#include <algorithm>
#include <glm/common.hpp>
#include <iostream>

Box::Box(glm::vec3 const& min, glm::vec3 const& max, Color const& color, std::string const& name)
    : Shape{name, color}
, min_{min}
, max_{max}
{
    std::cout << "Box::Box() - constructing \"" << name_ << "\"\n";
}

Box::~Box()
{
    std::cout << "Box::~Box() - destructing \"" << name_ << "\"\n";
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


HitPoint Box::intersect(Ray const& ray) const
{
    HitPoint result;

    glm::vec3 inv_dir{1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z};

    glm::vec3 t1 = (min_ - ray.origin) * inv_dir;
    glm::vec3 t2 = (max_ - ray.origin) * inv_dir;

    glm::vec3 t_min_axes = glm::min(t1, t2);
    glm::vec3 t_max_axes = glm::max(t1, t2);

    float t_near = std::max({t_min_axes.x, t_min_axes.y, t_min_axes.z});
    float t_far = std::min({t_max_axes.x, t_max_axes.y, t_max_axes.z});

    if (t_near > t_far || t_far < 0.0f) {
        result.hit = false;
        return result;
    }

    float distance = (t_near >= 0.0f) ? t_near : t_far;

    result.hit = true;
    result.distance = distance;
    result.name = name_;
    result.color = color_;
    result.intersection_point = ray.origin + distance * ray.direction;
    result.direction = ray.direction;

    return result;
}