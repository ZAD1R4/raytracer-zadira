#define GLM_ENABLE_EXPERIMENTAL
#include "sphere.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>

Sphere::Sphere(glm::vec3 const& center, float radius, std::shared_ptr<Material> const& material, std::string const& name)
    : Shape{name, material}
, center_{center}
, radius_{radius}
{
    std::cout << "Sphere::Sphere() - constructing \"" << name_ << "\"\n";
}

Sphere::~Sphere()
{
    std::cout << "Sphere::~Sphere() - destructing \"" << name_ << "\"\n";
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

    // Sicherstellen, dass die Strahlrichtung normiert ist
    glm::vec3 ray_dir = glm::normalize(ray.direction);

    float distance = 0.0f;
    bool did_hit = glm::intersectRaySphere(
        ray.origin, ray.direction,
        center_,
        radius_ * radius_,
        distance);

    result.hit = did_hit;

    if (did_hit && distance > 0.0f) {
        result.hit = true;
        result.distance = distance;
        result.name = name_;
        result.material = material_;
        result.color = material_ ? material_->kd : Color{0.0f, 0.0f, 0.0f};
        result.intersection_point = ray.origin + distance * ray.direction;
        result.direction = ray.direction;
        // Schnittpunkt berechnen
        result.intersection_point = ray.origin + distance * ray_dir;

        // Normale berechnen (Vektor vom Mittelpunkt zum Schnittpunkt, normiert)
        result.normal = glm::normalize(result.intersection_point - center_);
    } else {
        result.hit = false;
    }


    return result;
}