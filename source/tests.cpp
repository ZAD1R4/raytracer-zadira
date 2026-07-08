#define DOCTEST_CONFIG_IMPLEMENT
#define GLM_ENABLE_EXPERIMENTAL

#include "sphere.hpp"
#include "box.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "sdf_loader.hpp"
#include "doctest.h"
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "ray.hpp"
#include "hitpoint.hpp"
#include <glm/gtx/intersect.hpp>
#include <memory>

#include "glm/ext/scalar_constants.hpp"

namespace {
    std::shared_ptr<Material> make_material(std::string const& name, Color const& c, float m = 20.0f)
    {
        auto mat = std::make_shared<Material>();
        mat->name = name;
        mat->ka = c;
        mat->kd = c;
        mat->ks = c;
        mat->m = m;
        return mat;
    }
}

TEST_CASE("Sphere area and volume") {
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 2.0f, red, "sphere0"};

    CHECK(s.area() == doctest::Approx(4.0f * glm::pi<float>() * 4.0f));
    CHECK(s.volume() == doctest::Approx((4.0f / 3.0f) * glm::pi<float>() * 8.0f));
}

TEST_CASE("Sphere with radius zero") {
    auto green = make_material("green", Color{0.0f, 1.0f, 0.0f});
    Sphere s{glm::vec3{1.0f, 2.0f, 3.0f}, 0.0f, green, "sphere1"};

    CHECK(s.area() == doctest::Approx(0.0f));
    CHECK(s.volume() == doctest::Approx(0.0f));
}

TEST_CASE("Box area and volume") {
    auto blue = make_material("blue", Color{0.0f, 0.0f, 1.0f});
    Box b{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{2.0f, 3.0f, 4.0f}, blue, "box0"};

    CHECK(b.area() == doctest::Approx(2.0f * (2.0f*3.0f + 3.0f*4.0f + 4.0f*2.0f)));
    CHECK(b.volume() == doctest::Approx(24.0f));
}

TEST_CASE("Shape print via operator<<") {
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f, red, "test_sphere"};

    std::ostringstream oss;
    oss << s;

    CHECK(oss.str().find("test_sphere") != std::string::npos);
}

TEST_CASE("Sphere and Box print includes material name") {
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Sphere s{glm::vec3{1.0f, 2.0f, 3.0f}, 5.0f, red, "my_sphere"};

    std::ostringstream oss;
    oss << s;

    CHECK(oss.str().find("my_sphere") != std::string::npos);
    CHECK(oss.str().find("radius") != std::string::npos);
    CHECK(oss.str().find("\"red\"") != std::string::npos);
}

TEST_CASE("Polymorphic print via base pointer") {
    auto blue = make_material("blue", Color{0.0f, 0.0f, 1.0f});
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

    bool did_hit = glm::intersectRaySphere(
        ray_origin, ray_direction,
        sphere_center,
        sphere_radius * sphere_radius,
        distance);

    REQUIRE(did_hit);
    REQUIRE(distance == doctest::Approx(4.0f));
}

TEST_CASE("Sphere intersect: ray hits sphere head-on")
{
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Sphere s{glm::vec3{0.0f, 0.0f, 5.0f}, 1.0f, red, "test_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == true);
    CHECK(hp.distance == doctest::Approx(4.0f));
    CHECK(hp.name == "test_sphere");
    CHECK(hp.color.r == doctest::Approx(1.0f)); // kommt jetzt aus material->kd
}

TEST_CASE("Sphere intersect: ray misses sphere")
{
    auto blue = make_material("blue", Color{0.0f, 0.0f, 1.0f});
    Sphere s{glm::vec3{10.0f, 10.0f, 10.0f}, 1.0f, blue, "far_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == false);
}

TEST_CASE("Sphere intersect: ray origin inside sphere")
{
    auto green = make_material("green", Color{0.0f, 1.0f, 0.0f});
    Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 5.0f, green, "big_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 0.0f, 0.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == true);
    CHECK(hp.distance == doctest::Approx(5.0f));
}

TEST_CASE("Sphere intersect: sphere is behind the ray")
{
    auto yellow = make_material("yellow", Color{1.0f, 1.0f, 0.0f});
    Sphere s{glm::vec3{0.0f, 0.0f, -5.0f}, 1.0f, yellow, "behind_sphere"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = s.intersect(ray);

    CHECK(hp.hit == false);
}

TEST_CASE("Box intersect: ray hits box head-on from outside")
{
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
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
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Box b{glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, red, "test_box"};

    Ray ray{glm::vec3{10.0f, 10.0f, 5.0f}, glm::vec3{0.0f, 0.0f, -1.0f}};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == false);
}

TEST_CASE("Box intersect: ray origin inside the box")
{
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Box b{glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, red, "test_box"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == true);
    CHECK(hp.distance == doctest::Approx(1.0f));
}

TEST_CASE("Box intersect: box behind the ray")
{
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Box b{glm::vec3{-1.0f, -1.0f, -6.0f}, glm::vec3{1.0f, 1.0f, -4.0f}, red, "test_box"};

    Ray ray{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == false);
}

TEST_CASE("Box intersect: diagonal ray hits the box")
{
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
    Box b{glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, red, "test_box"};

    glm::vec3 dir = glm::normalize(glm::vec3{1.0f, 1.0f, 1.0f});
    Ray ray{glm::vec3{-5.0f, -5.0f, -5.0f}, dir};
    HitPoint hp = b.intersect(ray);

    CHECK(hp.hit == true);
}

TEST_CASE("Material operator<< prints name and coefficients")
{
    Material mat;
    mat.name = "red";
    mat.ka = Color{1.0f, 0.0f, 0.0f};
    mat.kd = Color{1.0f, 0.0f, 0.0f};
    mat.ks = Color{1.0f, 0.0f, 0.0f};
    mat.m = 20.0f;

    std::ostringstream oss;
    oss << mat;

    CHECK(oss.str().find("red") != std::string::npos);
    CHECK(oss.str().find("20") != std::string::npos);
}

TEST_CASE("Two shapes can share the same Material instance")
{
    auto shared = make_material("shared_red", Color{1.0f, 0.0f, 0.0f});

    Sphere s{glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f, shared, "sphere_a"};
    Box b{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, shared, "box_a"};

    CHECK(shared.use_count() == 3);
}

TEST_CASE("load_sdf: parses the example material definitions from the task sheet")
{
    std::string const tmp_path = "test_materials.sdf";
    {
        std::ofstream out(tmp_path);
        out << "define material red 1 0 0 1 0 0 1 0 0 20\n";
        out << "define material green 0 1 0 0 1 0 0 1 0 50\n";
        out << "define material blue 0 0 1 0 0 1 0 0 1 10\n";
    }

    Scene scene;
    load_sdf(tmp_path, scene);

    CHECK(scene.materials.size() == 3);
    REQUIRE(scene.materials.count("red") == 1);
    REQUIRE(scene.materials.count("green") == 1);
    REQUIRE(scene.materials.count("blue") == 1);

    auto const& red = scene.materials["red"];
    CHECK(red->ka.r == doctest::Approx(1.0f));
    CHECK(red->ka.g == doctest::Approx(0.0f));
    CHECK(red->kd.r == doctest::Approx(1.0f));
    CHECK(red->ks.r == doctest::Approx(1.0f));
    CHECK(red->m == doctest::Approx(20.0f));

    auto const& green = scene.materials["green"];
    CHECK(green->kd.g == doctest::Approx(1.0f));
    CHECK(green->m == doctest::Approx(50.0f));

    auto const& blue = scene.materials["blue"];
    CHECK(blue->ks.b == doctest::Approx(1.0f));
    CHECK(blue->m == doctest::Approx(10.0f));

    std::remove(tmp_path.c_str());
}

TEST_CASE("load_sdf: materials can be shared by shapes afterwards")
{
    std::string const tmp_path = "test_materials2.sdf";
    {
        std::ofstream out(tmp_path);
        out << "# Kommentarzeile sollte ignoriert werden\n";
        out << "define material red 1 0 0 1 0 0 1 0 0 20\n";
    }

    Scene scene;
    load_sdf(tmp_path, scene);

    REQUIRE(scene.materials.count("red") == 1);
    auto red = scene.materials["red"];

    scene.shapes.push_back(std::make_shared<Sphere>(glm::vec3{0,0,0}, 1.0f, red, "s1"));
    scene.shapes.push_back(std::make_shared<Box>(glm::vec3{0,0,0}, glm::vec3{1,1,1}, red, "b1"));

    CHECK(red.use_count() == 6);

    std::remove(tmp_path.c_str());
}

TEST_CASE("load_sdf: missing file does not crash and leaves the scene empty")
{
    Scene scene;
    load_sdf("this_file_does_not_exist.sdf", scene);

    CHECK(scene.materials.empty());
}

TEST_CASE("Static vs dynamic type example")
{
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
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
    auto red = make_material("red", Color{1.0f, 0.0f, 0.0f});
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