#pragma once

#include "shape.hpp"
#include <glm/vec3.hpp>

class Box : public Shape
{
public:
    Box(glm::vec3 const& min, glm::vec3 const& max);

    float area() const override;
    float volume() const override;

private:
    glm::vec3 min_;
    glm::vec3 max_;
};