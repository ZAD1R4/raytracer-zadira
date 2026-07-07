#pragma once

#include "material.hpp"
#include "shape.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>


struct Scene
{
    std::vector<std::shared_ptr<Shape>> shapes;
    std::map<std::string, std::shared_ptr<Material>> materials;
};