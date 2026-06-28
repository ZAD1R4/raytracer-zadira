#pragma once

#include "color.hpp"
#include <glm/vec3.hpp>
#include <string>

struct HitPoint
{
    bool hit = false;
    float distance = 0.0f;
    std::string name;
    Color color = {0.0f, 0.0f, 0.0f};
    glm::vec3 intersection_point = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};
};