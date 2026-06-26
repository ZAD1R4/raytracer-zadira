#define DOCTEST_CONFIG_IMPLEMENT

#include "sphere.hpp"
#include "box.hpp"
#include "doctest.h"
#include <glm/glm.hpp>

#include "glm/ext/scalar_constants.hpp"

TEST_CASE("Sphere area and volume") {
  Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 2.0f};

  CHECK(s.area() == doctest::Approx(4.0f * glm::pi<float>() * 4.0f));
  CHECK(s.volume() == doctest::Approx((4.0f / 3.0f) * glm::pi<float>() * 8.0f));
}

TEST_CASE("Sphere with radius zero")
{
  Sphere s{glm::vec3{1.0f, 2.0f, 3.0f}, 0.0f};

  CHECK(s.area() == doctest::Approx(0.0f));
  CHECK(s.volume() == doctest::Approx(0.0f));
}

TEST_CASE("Box area and volume - unit cube")
{
  Box b{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}};

  CHECK(b.area() == doctest::Approx(6.0f));
  CHECK(b.volume() == doctest::Approx(1.0f));
}

TEST_CASE("Box area and volume - non-cubic")
{
  Box b{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{2.0f, 3.0f, 4.0f}};

  CHECK(b.area() == doctest::Approx(2.0f * (2.0f*3.0f + 3.0f*4.0f + 4.0f*2.0f)));
  CHECK(b.volume() == doctest::Approx(24.0f));
}

int main(int argc, char *argv[])
{
  doctest::Context ctx;
  ctx.applyCommandLine(argc, argv);
  return ctx.run();
}
