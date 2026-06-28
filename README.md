Programmiersprachen: Raytracer
===========

[![Build Linux](../../actions/workflows/build-linux.yml/badge.svg)](../../actions/workflows/build-linux.yml)
[![Build macOS](../../actions/workflows/build-macos.yml/badge.svg)](../../actions/workflows/build-macos.yml)
[![Build Windows](../../actions/workflows/build-windows.yml/badge.svg)](../../actions/workflows/build-windows.yml)
[![Doctest](../../actions/workflows/doctest.yml/badge.svg)](../../actions/workflows/doctest.yml)

Framework for the **Miniproject Raytracer** of the Programming Languages course (*Programmiersprachen*).
Students work in groups of two to implement a ray tracing system that reads scenes in SDF format, supports geometric primitives (axis-aligned boxes and spheres), materials, point lights with shadows, and tone mapping.
The framework provides an OpenGL-based rendering window (via GLFW and NanoVG), math utilities (via GLM), and [doctest](https://github.com/doctest/doctest) for unit testing.


    // override forces the compiler to statically verify that the
    // method actually overrides the existing virtual method of the base class.
    // Without override, a typo in the signature (parameter types, const, name)
    // does not lead to a compilation error, but to the silent creation of a new method
    // unrelated to the base class, which violates the expected polymorphism
    // without any warning at the compilation stage.