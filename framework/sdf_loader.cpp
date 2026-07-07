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

            scene.materials[material->name] = material;
        }
    }
}