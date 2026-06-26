#pragma once

#include "shape.hpp"
#include <glm/vec3.hpp>

class Box : public Shape
{
public:
    Box(glm::vec3 const& min, glm::vec3 const& max, Color const& color, std::string const& name);

    float area() const override;
    float volume() const override;

private:
    glm::vec3 min_;
    glm::vec3 max_;
};