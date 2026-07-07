#pragma once

#include "hitpoint.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <iostream>
#include <memory>
#include <string>

class Shape
{
public:
    Shape(std::string const& name, std::shared_ptr<Material> const& material);
    virtual ~Shape();

    virtual float area() const = 0;
    virtual float volume() const = 0;

    virtual HitPoint intersect(Ray const& ray) const = 0;

    virtual std::ostream& print(std::ostream& os) const;

protected:
    std::string name_;
    std::shared_ptr<Material> material_;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);