#pragma once

#include "shape.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape
{
public:
    Sphere(glm::vec3 const& center, float radius);

    float area() const override;
    float volume() const override;

private:
    glm::vec3 center_;
    float radius_;
};