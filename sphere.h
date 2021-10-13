#include "TXLib.h"
#include <cmath>
#include "MathVector2D.h"
#include "MathVector3D.h"

using std::rand;
using std::max;

const double dt = 0.03;

typedef MathVector3D<double> vec3d;
typedef MathVector3D<double> vec_c;

enum OBJECT_TYPES {
    NOT_SPECIFIED = -1,
    TYPE_CIRCLE = 0,
    TYPE_RECT = 1,
    TYPE_SPHERE = 2,
    TYPE_COUNT = 3
};

enum WINDOW_TYPES {
    TYPE_WINDOW = 1,
    TYPE_TEXTURE = 2,
};

class BasicPhys;
class Window;
class Renderer;
class MainWindow;
class Frame;
class VirtualWindow;

template <typename T>
class Pair {
  public:
    Pair() = default;
    Pair(T x, T y) : x(x), y(y) {};
    T x;
    T y;
};

class Coordinates {
  public:
    Coordinates(double min_x, double max_x, double min_y, double max_y) : max(max_x, max_y), min(min_x, min_y) {};

    int check_x(double x) const;
    int check_y(double y) const;
    
    double get_max_x() const {return max.x;};    
    double get_min_x() const {return min.x;};
    double get_max_y() const {return max.y;};    
    double get_min_y() const {return min.y;};

  private:
    Pair<double> max;
    Pair<double> min;  
};

class Renderer {
  public:
    Renderer(VirtualWindow* window, Coordinates* coord);

    void set_window(VirtualWindow* new_window);
    VirtualWindow* get_window() const { return window; };
    
    void set_coordinates(Coordinates* new_coordinates) { coordinates = new_coordinates; };
    Coordinates* get_coordinates() const { return coordinates; };
    
    void clear() const;
    void draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color = TX_BLACK, int thickness = 1) const;
    void set_pixel(double x, double y, COLORREF color) const;
    void draw_circle(double x, double y, double r, COLORREF color = TX_BLACK, int thickness = 1) const;
    void draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color = TX_BLACK, int thinkness = 1) const;

    int to_pixel_x(double coord) const;
    int to_pixel_y(double coord) const;
    
    double to_coord_x(int x) const;
    double to_coord_y(int y) const;

    double get_scale_x() const { return scale.x; };
    double get_scale_y() const { return scale.y; };  

  private:
    VirtualWindow* window;
    Coordinates* coordinates;
    Pair<double> scale;
};

class Shape {
  public:
    Shape(double x, double y) : coord(x, y), shape_type(NOT_SPECIFIED) {};
    virtual void draw(const Renderer& renderer) const = 0;

    double get_x() const {return coord.x;};
    double get_y() const {return coord.y;};

    void set_x(double x) { coord.x = x; };
    void set_y(double y) { coord.y = y; };

    int get_type() const {return shape_type;};

  protected:
    Pair<double> coord;
    int shape_type;
};

class Sphere : public Shape {
  public:
    Sphere(double x, double y, double r, COLORREF color);

    double get_radius() const { return radius; };
    virtual void draw(const Renderer& renderer) const override;
    bool check_bound(double x, double y) const {
      return (((x - coord.x) * (x - coord.x) + (y - coord.y) * (y - coord.y)) <= (radius * radius));
    };

  protected:
    double radius;
    COLORREF color;
};


//z-axis is points towards us.
class PhongSphere : public Sphere {
  public:
    PhongSphere(double x, double y, double r, COLORREF color, COLORREF ambient, COLORREF light, vec3d light_pos, vec3d view_pos);
    virtual void draw(const Renderer& renderer) const override;

    void set_view(vec3d new_view) { view_pos = new_view; };
    void set_light(vec3d new_light) { light_pos = new_light; };

    vec3d get_view() const { return view_pos; };
    vec3d get_light() const { return light_pos; };

  private:
    vec_c ambient_intense;
    vec_c light_intense;
    vec3d light_pos;
    vec3d view_pos;
};


class Frame {
  public:
    Frame(int x_size, int y_size) : size(x_size, y_size) {};

    int get_size_x() const;
    int get_size_y() const;

  protected:
    Pair<int> size;
};

class VirtualWindow : public Frame {
  public:
    VirtualWindow(int x_size, int y_size, COLORREF color);
    
    HDC get_hdc() const;
    RGBQUAD* get_buf() const;
    COLORREF get_color() const;
    int get_type() const { return type; };

  protected:
    int type;
    HDC screen;
    RGBQUAD* buf_screen;
    COLORREF color;
};

class Window : public VirtualWindow {
  public:
    Window(int x_size, int y_size, COLORREF color);
};

class Texture : public VirtualWindow {
  public:
    Texture(int x_size, int y_size, COLORREF color, int coord_x, int coord_y);
    ~Texture();
    
    int get_coord_x() const;
    int get_coord_y() const;
    
    void show_on(const Window* window) const;

  private:
    Pair<int> coord;
};


RGBQUAD ToRGBQUAD(COLORREF color);
RGBQUAD ToRGBQUAD(BYTE red, BYTE green, BYTE blue);
vec_c ToVec(COLORREF color);
COLORREF ToCOLORREF(vec_c color_vec);
double CorrectColor(double color);