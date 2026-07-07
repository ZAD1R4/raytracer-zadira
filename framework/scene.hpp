#pragma once

#include "material.hpp"
#include "shape.hpp"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

//Global: Vergleichsoperator vor dem std::set deklariert
bool operator<(std::shared_ptr<Material> const& lhs, std::shared_ptr<Material> const& rhs);

struct Scene
{
    std::vector<std::shared_ptr<Shape>> shapes;
    std::map<std::string, std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Material>> materials_vector;
    std::set<std::shared_ptr<Material>>    materials_set;
};
// Die Deklarationen deiner drei Suchfunktionen
std::shared_ptr<Material> find_material_in_vector(std::vector<std::shared_ptr<Material>> const& vec, std::string const& name);
std::shared_ptr<Material> find_material_in_set(std::set<std::shared_ptr<Material>> const& set, std::string const& name);
std::shared_ptr<Material> find_material_in_map(std::map<std::string, std::shared_ptr<Material>> const& map, std::string const& name);