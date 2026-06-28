#pragma once

#include "shape.hpp"
#include "ray.hpp"
#include "hitpoint.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape
{
public:
    Sphere(glm::vec3 const& center, float radius, Color const& color, std::string const& name);
    ~Sphere() override;

    float area() const override;
    float volume() const override;

    std::ostream& print(std::ostream& os) const override;

    HitPoint intersect(Ray const& ray) const;

private:
    glm::vec3 center_;
    float radius_;
};