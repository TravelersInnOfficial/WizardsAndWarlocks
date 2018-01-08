#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <math.h>

template<typename T>
class vector3d{

private:
public:
    // variables of vector
    T X, Y, Z;      
    
    /**
     * Creates and empty 3d vector of all 0;
    */
    vector3d() : X(0), Y(0), Z(0) {}
    
    /**
     * Creates and initializes 3d vector of same number
    */
    vector3d(T n) : X(n), Y(n), Z(n) {}

    /**
     * Creates and initializes 3d vector
    */
    vector3d(T nx, T ny, T nz) : X(nx), Y(ny), Z(nz) {}

    /**
     * Copy constructor
    */
    vector3d(const vector3d<T>& other) : X(other.X), Y(other.Y), Z(other.Z) {}

    /**
     * returns length of the vector
    */
    float length(){return sqrtf(X*X + Y*Y + Z*Z);}

    /**
     * normalize the vector
     */

    void normalize(){
        float l = length();
        if(l==0){
            X = 0;
            Y = 0;
            Z = 0;
        }
        else{
            X = X/l;
            Y = Y/l;
            Z = Z/l;
        }
    }

    /**
     * Sets X Value
    */

    void setX(T newX){ X = newX;}

    /**
     * Sets Y Value
    */

    void setY(T newY){ Y = newY;}

    /**
     * Sets Z Value
    */

    void setZ(T newZ){ Z = newZ;}

    //Operators
    vector3d<T> operator+(const T v) const { return vector3d<T>(X + v, Y + v, Z + v); }
    vector3d<T> operator-(const T v) const { return vector3d<T>(X - v, Y - v, Z - v); }
    vector3d<T> operator*(const T v) const { return vector3d<T>(X * v, Y * v, Z * v); }
    vector3d<T> operator/(const T v) const { return vector3d<T>(X / v, Y / v, Z / v); }
    vector3d<T> operator-(vector3d<T> v) const { return vector3d<T>(X-v.X, Y-v.Y, Z-v.Z); }
    vector3d<T> operator+(vector3d<T> v) const { return vector3d<T>(X+v.X, Y+v.Y, Z+v.Z); }
    vector3d<T> operator*(vector3d<T> v) const { return vector3d<T>(X*v.X, Y*v.Y, Z*v.Z); }
    vector3d<T> operator/(vector3d<T> v) const { return vector3d<T>(X/v.X, Y/v.Y, Z/v.Z); }

    void operator=(const vector3d<T> v) { X = v.X; Y = v.Y; Z = v.Z; }
    void operator*=(const T v) { X = X*v; Y = Y*v; Z = Z*v; }

    bool operator==(const vector3d<T> v) { if(v.X == X && v.Y == Y && v.Z == Z) return true; return false; }


    friend inline std::ostream& operator << (std::ostream &o,const vector3d<T> &v){
        o << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
        return o;
    }
};

typedef vector3d<float> vector3df;
typedef vector3d<int> vector3di;

#endif