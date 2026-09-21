// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <cmath>
#include <limits>
#include <algorithm>

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

HitPoint Renderer::trace_ray(Scene const& scene, Ray const& ray) const
{
  HitPoint closest_hit;
  closest_hit.distance = std::numeric_limits<float>::infinity();

  for (auto const& shape : scene.shapes) {
    HitPoint hit = shape->intersect(ray);
    if (hit.hit && hit.distance > 0.0001f && hit.distance < closest_hit.distance) {
      closest_hit = hit;
    }
  }
  return closest_hit;
}

Color Renderer::shade(Scene const& scene, HitPoint const& hit, Ray const& ray, int depth) const
{
  if (!hit.hit || !hit.material) {
    return Color{0.0f, 0.0f, 0.0f}; // Hintergrundfarben (Schwarz)
  }

  auto const& mat = hit.material;

  // Ambientes Licht
  Color color_hdr = mat->ka * scene.ambient;

  glm::vec3 N = glm::normalize(hit.normal);
  glm::vec3 V = glm::normalize(-ray.direction);

  //  Schleife über alle Lichtquellen
  for (auto const& light : scene.lights) {
    glm::vec3 light_dir_vec = light.pos - hit.intersection_point;
    float dist_to_light = glm::length(light_dir_vec);
    glm::vec3 L = glm::normalize(light_dir_vec);

    // Schattenstrahl testen (mit leichtem Offset gegen Self-Shadowing)
    Ray shadow_ray{hit.intersection_point + N * 0.001f, L};
    HitPoint shadow_hit = trace_ray(scene, shadow_ray);

    // Wenn ein Objekt den Weg zur Lichtquelle blockiert -> im Schatten
    if (shadow_hit.hit && shadow_hit.distance < dist_to_light) {
      continue;
    }

    // Diffuser Anteil (Lambertian)
    float n_dot_l = std::max(0.0f, glm::dot(N, L));
    Color light_intensity = light.color * light.brightness;
    Color diffuse = mat->kd * light_intensity * n_dot_l;

    // Spekularer Anteil (Phong)
    glm::vec3 R = glm::reflect(-L, N);
    float r_dot_v = std::max(0.0f, glm::dot(R, V));
    float spec_factor = std::pow(r_dot_v, mat->m);
    Color specular = mat->ks * light_intensity * spec_factor;

    color_hdr = color_hdr + diffuse + specular;
  }

  // Aufgabe 7.2
  if (depth > 0) {
    float reflectivity = (mat->ks.r + mat->ks.g + mat->ks.b) / 3.0f;

    if (reflectivity > 0.0f) {
      glm::vec3 incoming_dir = glm::normalize(ray.direction);
      glm::vec3 reflect_dir = glm::reflect(incoming_dir, N);

      Ray reflected_ray{hit.intersection_point + N * 0.001f, reflect_dir};

      HitPoint reflected_hit = trace_ray(scene, reflected_ray);
      Color reflected_color = shade(scene, reflected_hit, reflected_ray, depth - 1);

      color_hdr = color_hdr + reflectivity * reflected_color;
    }
  }

  // Aufgabe 7.3
  if (depth > 0 && mat->opacity < 1.0f) {
    glm::vec3 I = glm::normalize(ray.direction);

    glm::vec3 N_local = N;
    float eta = 1.0f / mat->refraction_index; // n_aussen / n_innen, n_Luft = 1.0

    if (glm::dot(N, I) > 0.0f) {
      N_local = -N;
      eta = mat->refraction_index; // n_innen / n_aussen
    }

    glm::vec3 refract_dir = glm::refract(I, N_local, eta);
    Color transmitted_color{0.0f, 0.0f, 0.0f};

    if (glm::length(refract_dir) > 0.0f) {
      Ray refracted_ray{hit.intersection_point - N_local * 0.001f, refract_dir};
      HitPoint refracted_hit = trace_ray(scene, refracted_ray);
      transmitted_color = shade(scene, refracted_hit, refracted_ray, depth - 1);
    } else {
      glm::vec3 reflect_dir = glm::reflect(I, N_local);
      Ray reflected_ray{hit.intersection_point + N_local * 0.001f, reflect_dir};
      HitPoint reflected_hit = trace_ray(scene, reflected_ray);
      transmitted_color = shade(scene, reflected_hit, reflected_ray, depth - 1);
    }

    color_hdr = mat->opacity * color_hdr + (1.0f - mat->opacity) * transmitted_color;
  }

  return color_hdr;
}

void Renderer::render(Scene const& scene)
{
  float fov_rad = scene.camera.fov_x * (glm::pi<float>() / 180.0f);
  float distance = (width_ / 2.0f) / std::tan(fov_rad / 2.0f);

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x, y);

      glm::vec3 dir{
        float(x) - float(width_) / 2.0f + 0.5f,
        float(y) - float(height_) / 2.0f + 0.5f,
        -distance
      };

      Ray ray{{0.0f, 0.0f, 0.0f}, glm::normalize(dir)};

      HitPoint hit = trace_ray(scene, ray);
      Color color_hdr = shade(scene, hit, ray, MAX_REFLECTION_DEPTH);
      Color color_ldr;
      color_ldr.r = color_hdr.r / (color_hdr.r + 1.0f);
      color_ldr.g = color_hdr.g / (color_hdr.g + 1.0f);
      color_ldr.b = color_hdr.b / (color_hdr.b + 1.0f);

      p.color = color_ldr;

      write(p);
    }
  }

  ppm_.save(filename_);
}

void Renderer::write(Pixel const& p)
{
  size_t buf_pos = (width_ * p.y + p.x);
  if (buf_pos >= color_buffer_.size()) {
    std::cerr << "Error Renderer::write: Pixel out of bounds: "
              << p.x << "," << p.y << std::endl;
  } else {
    color_buffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}