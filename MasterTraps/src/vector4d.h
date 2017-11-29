#ifndef VECTOR4d_H
#define VECTOR4d_H

template<typename T>
class vector4d{

private:
public:
    // variables of vector
    T X, Y, X2, Y2;      
    
    /**
     * Creates and empty 4d vector of all 0;
    */
    vector4d() : X(0), Y(0), X2(0), Y2(0){}
    
    /**
     * Creates and initializes 4d vector
    */
    vector4d(T nx, T ny, T nx2, T ny2) : X(nx), Y(ny), X2(nx2), Y2(ny2) {}

    /**
     * Copy constructor
    */
    vector4d(const vector4d<T>& other) : X(other.X), Y(other.Y), X2(other.X2), Y2(other.Y2){}

    /**
     * returns length of the vector
    */
    float length(){return sqrtf(X*X + Y*Y + X2*X2);}

    /**
     * Sets X Value
    */

    void setX(T newX){ X = newX;}

    /**
     * Sets Y Value
    */

    void setY(T newY){ Y = newY;}

    /**
     * Sets X2 Value
    */

    void setX2(T newX2){ X2 = newX2;}

    /**
     * Sets Y2 Value
    */

    void setY2(T newY2){ Y2 = newY2;}

    //Operators
    vector4d<T> operator*(const T v) const { return vector4d<T>(X * v, Y * v, X2 * v, Y2 * v); }
    vector4d<T> operator/(const T v) const { return vector4d<T>(X / v, Y / v, X2 / v, Y2 / v); }
};

typedef vector4d<float> vector4df;
typedef vector4d<int> vector4di;

#endif