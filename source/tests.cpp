#define DOCTEST_CONFIG_IMPLEMENT
#define GLM_ENABLE_EXPERIMENTAL

#include "sphere.hpp"
#include "box.hpp"
#include "doctest.h"
#include <glm/glm.hpp>
#include <sstream>
#include "ray.hpp"
#include "hitpoint.hpp"
#include <glm/gtx/intersect.hpp>
#include <memory>

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

TEST_CASE("intersect_ray_sphere")
{
    glm::vec3 ray_origin{0.0f, 0.0f, 0.0f};
    glm::vec3 ray_direction{0.0f, 0.0f, 1.0f};

    glm::vec3 sphere_center{0.0f, 0.0f, 5.0f};
    float sphere_radius{1.0f};
    float distance = 0.0f;

    auto result = glm::intersectRaySphere(
        ray_origin, ray_direction,
        sphere_center,
        sphere_radius * sphere_radius,
        distance);

    REQUIRE(distance == doctest::Approx(4.0f));
}

TEST_CASE("Sphere intersect: ray hits sphere head-on")
{
    Color red{1.0f, 0.0f, 0.0f};
    Sphere s{glm::vec3{0.0f, 0.0f, 5.0f}, 1.0f, red, "test_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == true);
    CHECK(hp.distance == doctest::Approx(4.0f));
    CHECK(hp.name == "test_sphere");
    CHECK(hp.intersection_point.x == doctest::Approx(0.0f));
    CHECK(hp.intersection_point.y == doctest::Approx(0.0f));
    CHECK(hp.intersection_point.z == doctest::Approx(4.0f));
}

TEST_CASE("Sphere intersect: ray misses sphere")
{
    Color blue{0.0f, 0.0f, 1.0f};
    Sphere s{glm::vec3{10.0f, 10.0f, 10.0f}, 1.0f, blue, "far_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == false);
}

TEST_CASE("Sphere intersect: ray origin inside sphere")
{
    Color green{0.0f, 1.0f, 0.0f};
    Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 5.0f, green, "big_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 0.0f, 0.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == true);
    CHECK(hp.distance == doctest::Approx(5.0f));
}

TEST_CASE("Sphere intersect: ray tangent to sphere")
{
    Color white{1.0f, 1.0f, 1.0f};
    Sphere s{glm::vec3{0.0f, 1.0f, 5.0f}, 1.0f, white, "tangent_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = s.intersect(ray);

    MESSAGE("Tangent ray hit status: ", hp.hit);
}

TEST_CASE("Sphere intersect: sphere is behind the ray")
{
    Color yellow{1.0f, 1.0f, 0.0f};
    Sphere s{glm::vec3{0.0f, 0.0f, -5.0f}, 1.0f, yellow, "behind_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == false);
}

// Aufgabe 7.3: Box::intersect

TEST_CASE("Box intersect: ray hits box head-on from outside")
{
    Color red{1.0f, 0.0f, 0.0f};
    Box b{glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, red, "test_box"};

    Ray ray{glm::vec3{0.0f, 0.0f, 5.0f}, glm::vec3{0.0f, 0.0f, -1.0f}};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == true);
    CHECK(hp.distance == doctest::Approx(4.0f));
    CHECK(hp.name == "test_box");
    CHECK(hp.intersection_point.z == doctest::Approx(1.0f));
}

TEST_CASE("Box intersect: ray misses box")
{
    Color red{1.0f, 0.0f, 0.0f};
    Box b{glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, red, "test_box"};

    Ray ray{glm::vec3{10.0f, 10.0f, 5.0f}, glm::vec3{0.0f, 0.0f, -1.0f}};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == false);
}

TEST_CASE("Box intersect: ray origin inside the box")
{
    Color red{1.0f, 0.0f, 0.0f};
    Box b{glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, red, "test_box"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == true);
    CHECK(hp.distance == doctest::Approx(1.0f));
}

TEST_CASE("Box intersect: box behind the ray")
{
    Color red{1.0f, 0.0f, 0.0f};
    Box b{glm::vec3{-1.0f, -1.0f, -6.0f}, glm::vec3{1.0f, 1.0f, -4.0f}, red, "test_box"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == false);
}

TEST_CASE("Box intersect: diagonal ray hits the box")
{
    Color red{1.0f, 0.0f, 0.0f};
    Box b{glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, red, "test_box"};

    glm::vec3 dir = glm::normalize(glm::vec3{1.0f, 1.0f, 1.0f});
    Ray ray{glm::vec3{-5.0f, -5.0f, -5.0f}, dir};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == true);
}

TEST_CASE("Static vs dynamic type example")
{
    Color red{1.0f, 0.0f, 0.0f};
    glm::vec3 position{0.0f, 0.0f, 0.0f};

    std::shared_ptr<Sphere> s1 =
        std::make_shared<Sphere>(position, 1.2f, red, "sphere0");

    std::shared_ptr<Shape> s2 =
        std::make_shared<Sphere>(position, 1.2f, red, "sphere1");

    std::ostringstream oss1, oss2;
    s1->print(oss1);
    s2->print(oss2);

    CHECK(oss1.str().find("Sphere") != std::string::npos);
    CHECK(oss2.str().find("Sphere") != std::string::npos);
}

TEST_CASE("Virtual destructor - construction and destruction order")
{
    Color red{1.0f, 0.0f, 0.0f};
    glm::vec3 position{0.0f, 0.0f, 0.0f};

    Sphere* s1 = new Sphere{position, 1.2f, red, "sphere0"};
    Shape* s2 = new Sphere{position, 1.2f, red, "sphere1"};

    s1->print(std::cout);
    std::cout << "\n";
    s2->print(std::cout);
    std::cout << "\n";

    delete s1;
    delete s2;
}

int main(int argc, char *argv[]) {
    doctest::Context ctx;
    ctx.applyCommandLine(argc, argv);
    return ctx.run();
}