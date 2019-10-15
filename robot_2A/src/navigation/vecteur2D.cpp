#include <math.h>
#include "vecteur2D.hpp"

Vecteur2D::Vecteur2D(float x, float y)
{
    vec_x = x;
    vec_y = y;
}

float Vecteur2D::x()
{
    return vec_x;
}

float Vecteur2D::y()
{
    return vec_y;
}

void Vecteur2D::translation(float tx, float ty)
{
    vec_x += tx;
    vec_y += ty;
}

float angle(float point_x, float point_y)
{
    if (point_y < 1e-10)
        return M_PI / 2;
    return atan(point_x / point_y);
}