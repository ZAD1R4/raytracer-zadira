#define DOCTEST_CONFIG_IMPLEMENT

#include "sphere.hpp"
#include "box.hpp"
#include "doctest.h"
#include <glm/glm.hpp>
#include <sstream>

#include "glm/ext/scalar_constants.hpp"

TEST_CASE("Sphere area and volume") {
    Color red{1.0f, 0.0f, 0.0f};
    Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 2.0f, red, "sphere0"};

    CHECK(s.area() == doctest::Approx(4.0f * glm::pi<float>() * 4.0f));
    CHECK(s.volume() == doctest::Approx((4.0f / 3.0f) * glm::pi<float>() * 8.0f));
}

TEST_CASE("Sphere with radius zero") {
    Color green{0.0f, 1.0f, 0.0f};
    Sphere s{glm::vec3{1.0f, 2.0f, 3.0f}, 0.0f, green, "sphere1"};

    CHECK(s.area() == doctest::Approx(0.0f));
    CHECK(s.volume() == doctest::Approx(0.0f));
}

TEST_CASE("Box area and volume") {
    Color blue{0.0f, 0.0f, 1.0f};
    Box b{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{2.0f, 3.0f, 4.0f}, blue, "box0"};

    CHECK(b.area() == doctest::Approx(2.0f * (2.0f*3.0f + 3.0f*4.0f + 4.0f*2.0f)));
    CHECK(b.volume() == doctest::Approx(24.0f));
}

TEST_CASE("Shape print via operator<<") {
    Color red{1.0f, 0.0f, 0.0f};
    Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f, red, "test_sphere"};

    std::ostringstream oss;
    oss << s;

    CHECK(oss.str().find("test_sphere") != std::string::npos);
}

TEST_CASE("Sphere and Box print includes specific data") {
    Color red{1.0f, 0.0f, 0.0f};
    Sphere s{glm::vec3{1.0f, 2.0f, 3.0f}, 5.0f, red, "my_sphere"};

    std::ostringstream oss;
    oss << s;

    CHECK(oss.str().find("my_sphere") != std::string::npos);
    CHECK(oss.str().find("radius") != std::string::npos);
}

TEST_CASE("Polymorphic print via base pointer") {
    Color blue{0.0f, 0.0f, 1.0f};
    Shape *shape_ptr = new Sphere{glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f, blue, "poly_sphere"};

    std::ostringstream oss;
    oss << *shape_ptr;
    CHECK(oss.str().find("Sphere") != std::string::npos);

    delete shape_ptr;
}

int main(int argc, char *argv[]) {
    doctest::Context ctx;
    ctx.applyCommandLine(argc, argv);
    return ctx.run();
}
