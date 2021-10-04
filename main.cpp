#include "sphere.h"
#include "MathVector3D.h"

int main() {
    Window main_window(800, 800, TX_BLACK);
    Coordinates coord(0, 800, 0, 800);
    Renderer render(&main_window, &coord);

    double x = 400, y = 400, z = 1500, r = 200;

    PhongSphere s(x, y, r, RGB(51, 101, 121), RGB(20, 20, 40), RGB(0, 50, 150), vec3d(-800, -800, 0), vec3d(x, y, z));

    double deg_h = 0, deg_v = 0, d_a = 0.1;
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        vec3d view = s.get_view();
        
        if (GetAsyncKeyState(VK_RIGHT)) { deg_h += d_a; } 
        if (GetAsyncKeyState (VK_LEFT)) { deg_h -= d_a; }
        if (GetAsyncKeyState (VK_UP))   { deg_v += d_a; }
        if (GetAsyncKeyState (VK_DOWN)) { deg_v -= d_a; }

        s.set_view(vec3d(x + z * sin(deg_h), y + z * sin(deg_v), z * cos(deg_h)));
        s.draw(render);

        txSleep(50);
    }
    return 0;
}