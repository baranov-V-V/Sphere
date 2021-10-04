#include "sphere.h"

/*
PhysCircle::PhysCircle(double x, double y, double radius, COLORREF color) : 
    Shape(x, y), radius(radius), color(color) {
    shape_type = TYPE_CIRCLE;
};
*/

Sphere::Sphere(double x, double y, double r, COLORREF color) : Shape(x, y), radius(r), color(color)  {};

PhongSphere::PhongSphere(double x, double y, double r, COLORREF color, COLORREF ambient, COLORREF light, vec3d light_pos, vec3d view_pos) :
    Sphere(x, y, r, color), ambient_intense(ToVec(ambient)), light_intense(ToVec(light)), light_pos(light_pos), view_pos(view_pos) {
    //std::cout << "Constructed: " << ambient_intense << " " << light_intense << std::endl;
};


RGBQUAD ToRGBQUAD(COLORREF color) {
    return {GetBValue(color), GetGValue(color), GetRValue(color), 0};
};

RGBQUAD ToRGBQUAD(BYTE red, BYTE green, BYTE blue) {
    return {blue, green, red, 0};
};

vec_c ToVec(COLORREF color) {
    double red = double(GetRValue(color));
    double green = double(GetGValue(color));
    double blue = double(GetBValue(color));
    //std::cout << "r: " << red << " g: " << green << " b: " << blue << std::endl;
    vec_c vec(red / 255, green / 255, blue / 255);
    return vec;
};

COLORREF ToCOLORREF(vec_c color_vec) {
    return RGB(color_vec.get_x() * 255, color_vec.get_y() * 255, color_vec.get_z() * 255);
};