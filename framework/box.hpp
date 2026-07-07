#pragma once

#include "shape.hpp"
#include <glm/vec3.hpp>

class Box : public Shape
{
public:
    Box(glm::vec3 const& min, glm::vec3 const& max, Color const& color, std::string const& name);
    ~Box() override;

    float area() const override;
    float volume() const override;

    std::ostream& print(std::ostream& os) const override;

    HitPoint intersect(Ray const& ray) const override;

private:
    glm::vec3 min_;
    glm::vec3 max_;
};