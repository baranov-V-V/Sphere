#include "TXLib.h"

#if !defined (_TX_VER) || (_TX_VER < 0x172a0000)
#error Must use TXLib.h version >= 1.72 to compile this.
#endif

//============================================================================

template <typename T = double>
struct Vector
    {
    T x, y, z;

    Vector();
    explicit Vector (T xyz);
    Vector (T x0, T y0, T z0);
    };

typedef Vector<> vec;

//----------------------------------------------------------------------------

template <typename T>
inline Vector<T>::Vector() :
    x (0),
    y (0),
    z (0)
    {}

//----------------------------------------------------------------------------

template <typename T>
inline Vector<T>::Vector (T x0, T y0, T z0) :
    x (x0),
    y (y0),
    z (z0)
    {}

//----------------------------------------------------------------------------

template <typename T>
inline Vector<T>::Vector (T xyz) :
    x (xyz),
    y (xyz),
    z (xyz)
    {}

//----------------------------------------------------------------------------
// Normalization

template <typename T>
inline double len (const Vector<T>& v)
    {
    return sqrt (v.x*v.x + v.y*v.y + v.z*v.z);
    }

template <typename T>
inline Vector<T> operator ! (const Vector<T>& v)
    {
    return v / len (v);
    }

//----------------------------------------------------------------------------

template <typename T>
inline Vector<T>& operator += (Vector<T>& a, const Vector<T>& b)
    {
    a.x += b.x; a.y += b.y; a.z += b.z;
    return a;
    }

//----------------------------------------------------------------------------

template <typename T>
inline Vector<T> operator + (const Vector<T>& a, const Vector<T>& b)
    {
    return Vector<T> (a.x+b.x, a.y+b.y, a.z+b.z);
    }

//----------------------------------------------------------------------------

template <typename T>
inline Vector<T> operator - (const Vector<T>& a, const Vector<T>& b)
    {
    return Vector<T> (a.x-b.x, a.y-b.y, a.z-b.z);
    }

//----------------------------------------------------------------------------
// Non-vector product

template <typename T>
inline Vector<T> operator * (const Vector<T>& a, const Vector<T>& b)
    {
    return Vector<T> (a.x*b.x, a.y*b.y, a.z*b.z);
    }

//----------------------------------------------------------------------------
// Scalar (dot) product

template <typename T>
inline T operator ^ (const Vector<T>& a, const Vector<T>& b)
    {
    return a.x*b.x + a.y*b.y + a.z*b.z;
    }

//----------------------------------------------------------------------------

template <typename T, typename T2>
inline Vector<T> operator * (const Vector<T>& v, T2 val)
    {
    return Vector<T> (v.x*val, v.y*val, v.z*val);
    }

template <typename T, typename T2>
inline Vector<T> operator * (T2 val, const Vector<T>& v)
    {
    return v * val;
    }

//----------------------------------------------------------------------------

template <typename T>
inline Vector<T> operator / (const Vector<T>& v, T val)
    {
    return Vector<T> (v.x/val, v.y/val, v.z/val);
    }

//----------------------------------------------------------------------------

template <typename T>
inline std::ostream& operator << (std::ostream& o, const Vector<T>& v)
    {
    o << "Vector (" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;
    }

//----------------------------------------------------------------------------

inline double Clamp (double value, double min, double max)
    {
    return (value < min)? min : (value > max)? max : value;
    }

//----------------------------------------------------------------------------

inline double Random (double min, double max)
    {
    return min + (max - min) * rand() / RAND_MAX;
    }

//============================================================================

void DrawScene (const vec& lightPos, double r);

vec PhongLightning (const vec& p, const vec& N, const vec& V,
                    const vec& lightPos, const vec& lightColor,
                    const vec& materialColor,
                    const vec& ambientColor = vec (0, 0, 0));

const vec Bump (const vec& p = vec(), double r = 0);

void DrawPixel (double x, double y, Vector<double> color);

//----------------------------------------------------------------------------

bool UseTxPixel = false;

//----------------------------------------------------------------------------

int main()
    {
    double r = 200;

    txCreateWindow (800, 600);
    txTextCursor (false);
    txBegin();

        
    txSetTextAlign (TA_CENTER);
    txSetColor (TX_DARKGRAY);
    for (double t = txPI/2; ; t += 0.1)
        {
        DrawScene (vec(-2 * r, +1 * r, +2 * r), r);
        txSleep ();

        if (GetAsyncKeyState (VK_ESCAPE) && GetForegroundWindow() == txWindow()) break;
        }

    txEnd();
    return 0;
    }

//----------------------------------------------------------------------------

void DrawScene (const vec& lightPos, double r)
    {
    const vec viewPos       (   0,    0, +5*r);
//  const vec lightPos      (-2*r, +2*r, +2*r);

    const vec materialColor (0.0, 1.0, 0.0);
    const vec lightColor    (1.0, 0.7, 0.0);
    const vec ambientColor  (0.2, 0.2, 0.2);

    for (double y = +r; y >= -r; y--)
    for (double x = -r; x <= +r; x++) {

        vec p = vec (x, y, 0);
        if (p.x * p.x + p.y * p.y > r * r) { 
            DrawPixel (x, y, vec (0)); 
            continue; 
        }

        p.z = sqrt (r*r - p.x * p.x - p.y * p.y);
        vec N = !p;

        vec V = !(viewPos - p);
        vec L = !(lightPos - p);

        double diffuse = N ^ L;
        
        if (diffuse < 0) {
            diffuse = 0;
        }

        vec Lr = 2 * (N ^ L) * N - L;
        double spec = Lr ^ V;
        if (spec < 0) spec = 0;
        double specular = pow (spec, 5);

        vec color = ambientColor * materialColor              +

                    diffuse      * materialColor * lightColor + 
                    specular     *                 lightColor;
                                    
        DrawPixel (x, y, color);
        }
    }

//----------------------------------------------------------------------------

inline void DrawPixel (double x, double y, Vector<double> color)
    {
    static POINT scr = txGetExtent();

    POINT   p   = { (int) (x + scr.x/2 + 0.5),
                    (int) (y + scr.y/2 + 0.5) };

    RGBQUAD rgb = { (BYTE) (Clamp (color.x, 0, 1) * 255 + 0.5),
                    (BYTE) (Clamp (color.y, 0, 1) * 255 + 0.5),
                    (BYTE) (Clamp (color.z, 0, 1) * 255 + 0.5) };

    if (UseTxPixel)
         txSetPixel (p.x, p.y, RGB (rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue));
    else
         *(txVideoMemory() + p.x + (-p.y + scr.y) * scr.x) = rgb;
    }