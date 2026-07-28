#pragma once

#include "material.hpp"
#include "shape.hpp"
#include <map>
#include "color.hpp"
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

//Global: Vergleichsoperator vor dem std::set deklariert
bool operator<(std::shared_ptr<Material> const& lhs, std::shared_ptr<Material> const& rhs);

// Struktur für Punktlichtquellen (aus der SDF-Spezifikation)
struct Light
{
    std::string name;
    glm::vec3 pos{0.0f, 0.0f, 0.0f};
    Color color{1.0f, 1.0f, 1.0f};
    float brightness = 1.0f;
};

struct Camera
{
    std::string name = "eye";
    float fov_x = 45.0f; // Öffnungswinkel
    // Für Aufgabe 7.4 kommen hier später eye, dir, up hinzu
};

struct Scene
{
    // Objekt- und Material-Verwaltung
    std::map<std::string, std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Shape>> shapes;

    //Das sind nicht wichtig map reicht aus
    //std::vector<std::shared_ptr<Shape>> shapes;
    //std::vector<std::shared_ptr<Material>> materials_vector;
   // std::set<std::shared_ptr<Material>>    materials_set;

    // Beleuchtung
    std::vector<Light> lights;
    Color ambient{0.1f, 0.1f, 0.1f};

    // Kamera und Rendering-Parameter
    Camera camera;
    std::string output_filename = "image.ppm";
    int x_res = 480;
    int y_res = 320;
};

// Die Deklarationen deiner drei Suchfunktionen
std::shared_ptr<Material> find_material_in_vector(std::vector<std::shared_ptr<Material>> const& vec, std::string const& name);
std::shared_ptr<Material> find_material_in_set(std::set<std::shared_ptr<Material>> const& set, std::string const& name);
std::shared_ptr<Material> find_material_in_map(std::map<std::string, std::shared_ptr<Material>> const& map, std::string const& name);