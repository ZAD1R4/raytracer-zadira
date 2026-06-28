#pragma once

#include "shape.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape
{
public:
    Sphere(glm::vec3 const& center, float radius, Color const& color, std::string const& name);

    float area() const override;
    float volume() const override;

    std::ostream& print(std::ostream& os) const override;

    // override forces the compiler to statically verify that the
    // method actually overrides the existing virtual method of the base class.
    // Without override, a typo in the signature (parameter types, const, name)
    // does not lead to a compilation error, but to the silent creation of a new method
    // unrelated to the base class, which violates the expected polymorphism
    // without any warning at the compilation stage.

private:
    glm::vec3 center_;
    float radius_;
};