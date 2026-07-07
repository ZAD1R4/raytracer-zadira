#include "sdf_loader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

void load_sdf(std::string const& filepath, Scene& scene)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "load_sdf: konnte Datei nicht oeffnen: \"" << filepath << "\"\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword.empty() || keyword[0] == '#') {
            continue;
        }

        if (keyword != "define") {
            continue;
        }

        std::string object_class;
        iss >> object_class;

        if (object_class == "material") {
            auto material = std::make_shared<Material>();
            iss >> material->name
                >> material->ka.r >> material->ka.g >> material->ka.b
                >> material->kd.r >> material->kd.g >> material->kd.b
                >> material->ks.r >> material->ks.g >> material->ks.b
                >> material->m;

            if (!iss) {
                std::cerr << "load_sdf: fehlerhafte material-Zeile: \"" << line << "\"\n";
                continue;
            }

            scene.materials[material->name] = material; //Pushen für std::map
            scene.materials_vector.push_back(material); ///Pushen für std::vector
            scene.materials_set.insert(material);       //Pushen für std::set
        }
    }
}

//Aufgabe 7.6
//Definition "<" operators (Es wurde in scene.hpp als Global deklariert)
bool operator<(std::shared_ptr<Material> const& lhs, std::shared_ptr<Material> const& rhs) {
    return lhs->name < rhs->name;
}

//Suchfunktion Implementation
std::shared_ptr<Material> find_material_in_vector(std::vector<std::shared_ptr<Material>> const& vec, std::string const& name) {
    auto it = std::find_if(vec.begin(), vec.end(), [&name](auto const& mat) {
        return mat->name == name;
    });
    return (it != vec.end()) ? *it : nullptr;
}

std::shared_ptr<Material> find_material_in_set(std::set<std::shared_ptr<Material>> const& set, std::string const& name) {
    auto dummy_material = std::make_shared<Material>();
    dummy_material->name = name;

    auto it = set.find(dummy_material);
    return (it != set.end()) ? *it : nullptr;
}

std::shared_ptr<Material> find_material_in_map(std::map<std::string, std::shared_ptr<Material>> const& map, std::string const& name) {
    auto it = map.find(name);
    return (it != map.end()) ? it->second : nullptr;
}