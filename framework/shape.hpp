#pragma once

class Shape
{
public:
    virtual ~Shape() = default;

    virtual float area() const = 0;
    virtual float volume() const = 0;
};