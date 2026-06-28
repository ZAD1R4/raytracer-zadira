Programmiersprachen: Raytracer
===========

[![Build Linux](../../actions/workflows/build-linux.yml/badge.svg)](../../actions/workflows/build-linux.yml)
[![Build macOS](../../actions/workflows/build-macos.yml/badge.svg)](../../actions/workflows/build-macos.yml)
[![Build Windows](../../actions/workflows/build-windows.yml/badge.svg)](../../actions/workflows/build-windows.yml)
[![Doctest](../../actions/workflows/doctest.yml/badge.svg)](../../actions/workflows/doctest.yml)

Framework for the **Miniproject Raytracer** of the Programming Languages course (*Programmiersprachen*).
Students work in groups of two to implement a ray tracing system that reads scenes in SDF format, supports geometric primitives (axis-aligned boxes and spheres), materials, point lights with shadows, and tone mapping.
The framework provides an OpenGL-based rendering window (via GLFW and NanoVG), math utilities (via GLM), and [doctest](https://github.com/doctest/doctest) for unit testing.


    Override forces the compiler to statically verify that the
    method actually overrides the existing virtual method of the base class.
    Without override, a typo in the signature (parameter types, const, name)
    does not lead to a compilation error, but to the silent creation of a new method
    unrelated to the base class, which violates the expected polymorphism
    without any warning at the compilation stage.

**Aufgabe 6.7**

    Statischer Typ:

    Der statische Typ einer Variable ist der Typ, der bei ihrer Deklaration im Quellcode angegeben wird.
    Er ist dem Compiler bereits zur Compile-Zeit bekannt und ändert sich während der Laufzeit des Programms nicht.

    Dynamischer Typ:

    Der dynamische Typ einer Variable ist der tatsächliche Typ des Objekts, auf das die Variable zur Laufzeit verweist.
    Er wird erst zur Laufzeit bestimmt, abhängig davon, welches konkrete Objekt tatsächlich erzeugt wurde (z. B. über new oder make_shared).
    
    Wann wird welcher Typ überprüft:
    
    Der statische Typ wird zur Compile-Zeit überprüft. 
    Der Compiler entscheidet anhand des statischen Typs, welche Methoden und Member über eine Variable überhaupt 
    aufgerufen werden dürfen.
    
    Der dynamische Typ wird zur Laufzeit ausgewertet. Er entscheidet bei virtuellen Methoden, welche konkrete (überschriebene) Implementierung tatsächlich ausgeführt wird.

