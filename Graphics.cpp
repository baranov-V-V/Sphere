//graphics engine

#include "sphere.h"

Window::Window(int x_size, int y_size, COLORREF color) : VirtualWindow(x_size, y_size, color) {    
    txCreateWindow(x_size, y_size);
    screen = txDC();
    buf_screen = txVideoMemory();
    type = TYPE_WINDOW;
    
    txSetFillColor(color, screen);
    txRectangle(0, 0, x_size, y_size, screen);
};

Texture::Texture(int x_size, int y_size, COLORREF color, int coord_x, int coord_y) :
    VirtualWindow(x_size, y_size, color), coord(coord_x, coord_y) {
    type = TYPE_TEXTURE;
    screen = txCreateDIBSection(x_size, y_size, &buf_screen);
    
    txSetFillColor(color, screen);
    txRectangle(0, 0, x_size, y_size, screen);
}

Texture::~Texture() {
    txDeleteDC(screen);
}

void Texture::show_on(const HDC& target) const {
    txBitBlt(target, coord.x, coord.y, 0, 0, screen);
}

void Renderer::draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txLine(this->to_pixel_x(x_begin), window->get_size_y() - this->to_pixel_y(y_begin),
           this->to_pixel_x(x_end), window->get_size_y() -  this->to_pixel_y(y_end), window->get_hdc());
};

void Renderer::set_pixel(double x, double y, COLORREF color) const {
    txSetPixel(this->to_pixel_x(x), window->get_size_y() - this->to_pixel_y(y), color, window->get_hdc());
};

void Renderer::draw_circle(double x, double y, double r, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txEllipse(this->to_pixel_x(x - r), window->get_size_y() - this->to_pixel_y(y + r),
              this->to_pixel_x(x + r), window->get_size_y() - this->to_pixel_y(y - r), window->get_hdc());
};

void Renderer::draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txRectangle(this->to_pixel_x(x1), window->get_size_y() - this->to_pixel_y(y1),
                this->to_pixel_x(x2), window->get_size_y() - this->to_pixel_y(y2), window->get_hdc());
};

void Renderer::clear() const {
    txClear(window->get_hdc());
};

void Sphere::draw(const Renderer& renderer) const {
    VirtualWindow* window = renderer.get_window();
    Coordinates* coordinates = renderer.get_coordinates(); 
    RGBQUAD* win_buf = window->get_buf();
    RGBQUAD place_color = ToRGBQUAD(color);

    int x1 = renderer.to_pixel_x(coord.x - radius);
    int x2 = renderer.to_pixel_x(coord.x + radius);
    int y1 = renderer.to_pixel_y(coord.y + radius);
    int y2 = renderer.to_pixel_y(coord.y - radius);
    
    for(int iy = y2; iy < y1; ++iy) {
        for(int ix = x1; ix < x2; ++ix) {
            int buf_pos = ix + iy * window->get_size_x();
            if (this->check_bound(renderer.to_coord_x(ix), renderer.to_coord_y(iy)) &&
                (buf_pos < window->get_size_x() * window->get_size_y()) && buf_pos >= 0) {
                win_buf[buf_pos] = place_color;
            }
        }
    }
};

void PhongSphere::draw(const Renderer& renderer) const {
    VirtualWindow* window = renderer.get_window();
    Coordinates* coordinates = renderer.get_coordinates(); 
    RGBQUAD* win_buf = window->get_buf();

    int x1 = renderer.to_pixel_x(coord.x - radius);
    int x2 = renderer.to_pixel_x(coord.x + radius);
    int y1 = renderer.to_pixel_y(coord.y + radius);
    int y2 = renderer.to_pixel_y(coord.y - radius);
    
    //cout << "normal: " << normal << std::endl;
    //cout << "ambient: " << ambient_intense << "light: " << light_intense << std::endl;

    for(int iy = y2; iy < y1; ++iy) {
        for(int ix = x1; ix < x2; ++ix) {
            int buf_pos = ix + iy * window->get_size_x();
            if (!this->check_bound(renderer.to_coord_x(ix), renderer.to_coord_y(iy))) {
                continue;
            }
            double x = renderer.to_coord_x(ix) - coord.x;
            double y = renderer.to_coord_y(iy) - coord.y; 


            vec3d normal(x, y, sqrt(radius * radius - x * x - y * y));
            vec3d n_norm = ~normal;
            vec3d v_norm = ~(view_pos - normal);
            vec3d lm_norm = ~(light_pos - normal);
            vec3d rm_norm = 2 * (lm_norm * n_norm) * n_norm - lm_norm;

            double k_diff = n_norm * lm_norm;
            double product = rm_norm * v_norm;
            //cout << "k_diff: " << k_diff << "k_spec: " << spec << std::endl;
            
            if (k_diff < 0) {
                k_diff = 0;
            }
            if (product < 0) {
                product = 0;
            }
            double k_spec = pow(product, 30);
            
            vec_c sphere_color = ToVec(color);
            double red = ambient_intense.get_x() * sphere_color.get_x() + k_diff * light_intense.get_x() * sphere_color.get_x() + k_spec * light_intense.get_x();
            double green = ambient_intense.get_y() * sphere_color.get_y() + k_diff * light_intense.get_y() * sphere_color.get_y() + k_spec * light_intense.get_y();
            double blue = ambient_intense.get_z() * sphere_color.get_z() + k_diff * light_intense.get_z() * sphere_color.get_z() + k_spec * light_intense.get_z();
            if ((buf_pos < window->get_size_x() * window->get_size_y()) && buf_pos >= 0) {
                win_buf[buf_pos] = ToRGBQUAD(CorrectColor(red) * 255, CorrectColor(green) * 255, CorrectColor(blue) * 255);
            }
        }
    }

};

double CorrectColor(double color) {
    assert(isfinite(color));
    if (color >= 1) {
        color = 1;
    }
    if (color <= 0) {
        color = 0;
    }
    return color;
}


