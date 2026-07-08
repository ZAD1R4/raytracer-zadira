#pragma once

#include "color.hpp"
#include <iostream>
#include <string>

struct Material
{
    std::string name;
    // ambient/diffuse/specular
    Color ka{0.0f, 0.0f, 0.0f};
    Color kd{0.0f, 0.0f, 0.0f};
    Color ks{0.0f, 0.0f, 0.0f};
    float m = 0.0f;
};

std::ostream& operator<<(std::ostream& os, Material const& mat);