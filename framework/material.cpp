#include "material.hpp"

std::ostream& operator<<(std::ostream& os, Material const& mat)
{
    os << "Material \"" << mat.name << "\""
       << ", ka = " << mat.ka
       << ", kd = " << mat.kd
       << ", ks = " << mat.ks
       << ", m = " << mat.m;
    return os;
}