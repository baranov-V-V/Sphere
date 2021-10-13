#include "sphere.h"
#include "MathVector3D.h"

int main() {
    Window main_window(800, 800, TX_BLACK);
    Texture texture(800, 800, TX_BLACK, 0, 0);
    Coordinates coord(0, 8, 0, 8);
    Renderer render(&texture, &coord);

    double x = 4, y = 4, z = 15, r = 2;

    PhongSphere s(x, y, r, RGB(51, 101, 121), RGB(20, 20, 40), RGB(0, 50, 150), vec3d(-8, -8, 0), vec3d(x, y, z));

    double deg_h = 0, deg_v = 0, d_a = 0.1;
    double dx = 0.3, dy = 0.3;

    while (!GetAsyncKeyState(VK_ESCAPE)) {
        vec3d view = s.get_view();

        if (GetAsyncKeyState('D')) { s.set_x(s.get_x() + dx); } 
        if (GetAsyncKeyState('A')) { s.set_x(s.get_x() - dx); }
        if (GetAsyncKeyState('W')) { s.set_y(s.get_y() + dy); }
        if (GetAsyncKeyState('S')) { s.set_y(s.get_y() - dy); }

        if (GetAsyncKeyState(VK_RIGHT)){ deg_h += d_a; } 
        if (GetAsyncKeyState(VK_LEFT)) { deg_h -= d_a; }
        if (GetAsyncKeyState(VK_UP))   { deg_v += d_a; }
        if (GetAsyncKeyState(VK_DOWN)) { deg_v -= d_a; }
        
        s.set_view(vec3d(x + z * sin(deg_h), y + z * sin(deg_v), z * cos(deg_h)));
        s.draw(render);
        texture.show_on(&main_window);
        txSleep(30);
        render.clear();
    }

    return 0;
}