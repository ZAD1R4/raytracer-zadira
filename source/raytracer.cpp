#include "renderer.hpp"
#include "sdf_loader.hpp"
#include "scene.hpp"
#include "window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

int main(int argc, char* argv[])
{
  std::string sdf_path = "../scene.sdf"; // Standardpfad
  if (argc > 1) {
    sdf_path = argv[1];
  }

  std::cout << "=== RAYTRACER START ===" << std::endl;
  std::cout << "Lade Datei: " << sdf_path << std::endl;

  Scene scene;
  // Standard-FOV als Fallback setzen, falls nicht in SDF
  scene.camera.fov_x = 45.0f;

  load_sdf(sdf_path, scene);

  std::cout << "Geladene Objekte: " << scene.shapes.size() << std::endl;
  std::cout << "Geladene Lichter: " << scene.lights.size() << std::endl;
  std::cout << "Kamera FOV: "      << scene.camera.fov_x << std::endl;

  if (scene.shapes.empty()) {
    std::cerr << "WARNUNG: Keine Objekte in der Szene! Ueberpruefe den Dateipfad." << std::endl;
  }

  unsigned width  = static_cast<unsigned>(scene.x_res);
  unsigned height = static_cast<unsigned>(scene.y_res);

  Renderer renderer(width, height, scene.output_filename);

  std::cout << "Rendere Szene..." << std::endl;
  renderer.render(scene);
  std::cout << "Fertig gerendert!" << std::endl;

  Window window{{width, height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  return 0;
}