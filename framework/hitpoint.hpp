#pragma once

#include "color.hpp"
#include <glm/vec3.hpp>
#include <string>
#include <memory>

struct Material;

struct HitPoint
{
    bool hit = false;
    float distance = 0.0f;
    std::string name;
    Color color = {0.0f, 0.0f, 0.0f};
    glm::vec3 intersection_point = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};

    //Für das Beleuchtungsmodell. Normalvektor, der senkrecht auf der Oberfläche am Schnittpunkt steht.
    //hilft beim berechnen von Winkel des einfallenden Lichtes
    glm::vec3 normal = {0.0f, 0.0f, 0.0f};

    //Damit wir beim Berechnen der Farbe direkt auf die Materialkoeffizienten ($K_a, K_d, K_s, m$) zugreifen können.
    std::shared_ptr<Material> material = nullptr;
};