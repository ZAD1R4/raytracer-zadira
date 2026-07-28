#include "renderer.hpp"
#include "sdf_loader.hpp"
#include "scene.hpp"
#include "window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

int main(int argc, char* argv[])
{
  std::string sdf_path = "szene.sdf";
  if (argc > 1) {
    sdf_path = argv[1];
  }

  // 1. Szene laden
  Scene scene;
  load_sdf(sdf_path, scene);

  // 2. Renderer initialisieren
  Renderer renderer{scene.x_res, scene.y_res, scene.output_filename};

  // 3. Szene rendern
  renderer.render(scene);

  // 4. In ein Fenster ausgeben
  Window window{{scene.x_res, scene.y_res}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  return 0;
}