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

    // Aufgabe 7.3
    float refraction_index = 1.0f; // n —  1.0 (air), 1.5 (glass), 1.33 (water)
    float opacity = 1.0f;          // 1.0 = непрозрачный, 0.0 = прозрачный
};

std::ostream& operator<<(std::ostream& os, Material const& mat);