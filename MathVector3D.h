#ifndef MATH_VECTOR_3D
#define MATH_VECTOR_3D

template <typename T>
class MathVector3D
{   
public:
    void set_x(T new_x) {x = new_x;};
    void set_y(T new_y) {y = new_y;};
    void set_z(T new_z) {x = new_z;};

    T get_x() {return x;};
    T get_y() {return y;};
    T get_z() {return z;};

    T get_x() const {return x;};
    T get_y() const {return y;};
    T get_z() const {return z;};

    MathVector3D() = default;
    MathVector3D(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {};

    MathVector3D& operator+=(const MathVector3D& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    MathVector3D& operator-=(const MathVector3D& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    template <typename T2>
    MathVector3D& operator*=(T2 scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    template <typename T2>
    MathVector3D& operator/=(T2 scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    MathVector3D& operator~() {
        *this /= this->len();
        return *this;
    }

    void print(std::ostream& stream) { stream << "(" << x << ", " << y << ", " << z << ")"; }
    void print() { this->print(std::cout); };
    
    void print(std::ostream& stream) const { ((MathVector3D*)this)->print(stream); }
    void print() const { this->print(std::cout); };

    double len() { return sqrt(x * x + y * y + z * z); };

private:
    T x;
    T y;
    T z;
};

template <typename T>
MathVector3D<T> operator+(const MathVector3D<T>& lhs, const MathVector3D<T>& rhs) {
    MathVector3D<T> result(lhs);
    result += rhs;
    return result;
}

template <typename T>
MathVector3D<T> operator-(const MathVector3D<T>& lhs, const MathVector3D<T>& rhs) {
    MathVector3D<T> result(lhs);
    result -= rhs;
    return result;
}

template <typename T, typename T2>
MathVector3D<T> operator*(const MathVector3D<T>& vector, T2 scalar) {
    MathVector3D<T> result(vector);
    result *= scalar;
    return result;
}

template <typename T, typename T2>
MathVector3D<T> operator*(T2 scalar, const MathVector3D<T>& vector) {
    MathVector3D<T> result(vector);
    result *= scalar;
    return result;
}

template <typename T, typename T2>
MathVector3D<T> operator/(const MathVector3D<T>& vector, T2 scalar) {
    MathVector3D<T> result(vector);
    result /= scalar;
    return result;
}

template <typename T, typename T2>
MathVector3D<T> operator/(T2 scalar, const MathVector3D<T>& vector) {
    MathVector3D<T> result(vector);
    result /= scalar;
    return result;
}

template <typename T>
T operator*(const MathVector3D<T>& lhs, const MathVector3D<T>& rhs) {
    return lhs.get_x() * rhs.get_x() + lhs.get_y() * rhs.get_y() + lhs.get_z() * rhs.get_z();
}

template <typename T>
MathVector3D<T> operator&(const MathVector3D<T>& lhs, const MathVector3D<T>& rhs) {
    MathVector3D<T> result(lhs.get_x() * rhs.get_x(), lhs.get_y() * rhs.get_y(), lhs.get_z() * rhs.get_z());
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const MathVector3D<T>& vector) {
    vector.print(stream);
    return stream;
}

#endif