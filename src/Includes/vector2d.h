#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cmath>
#include <Constants.h>
#include <vector3d.h>

template<typename T>
class vector2d{

private:
public:
    // variables of vector
    T X, Y;      
    
    /**
     * Creates and empty 3d vector of all 0;
    */
    vector2d() : X(0), Y(0) {}
    
    /**
     * Creates and initializes 3d vector
    */
    vector2d(T nx, T ny) : X(nx), Y(ny) {}

    /**
     * Copy constructor
    */
    vector2d(const vector2d<T>& other) : X(other.X), Y(other.Y) {}

    /**
     * Convert 3d in 2d
     */

    vector2d(const vector3d<T>& other) : X(other.X), Y(other.Y) {}

    /**
     * returns length of the vector
    */
    float length(){return sqrtf(X*X + Y*Y);}

    /**
     * convert to abs the values of the vector
     */
    vector2d absolute(){
       return vector2d(abs(X), abs(Y));
    }

    /**
     * normalize the vector
     */

    void normalize(){
        float l = length();
        if(l==0){
            X = 0;
            Y = 0;
        }
        else{
            X = X/l;
            Y = Y/l;
        }
    }

    /**
     * Map the Angles between [-pi, pi]
     */

    void mapToRangePI(){
        if(X>M_PI){
            X = X - 2*M_PI;
        }else if(X<-M_PI){
            X = X + 2*M_PI;
        }
        if(Y>M_PI){
            Y = Y - 2*M_PI;
        }else if(Y<-M_PI){
            Y = Y + 2*M_PI;
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


    //Operators
    vector2d<T> operator*(const T v) const { return vector2d<T>(X * v, Y * v); }
    vector2d<T> operator/(const T v) const { return vector2d<T>(X / v, Y / v); }
    vector2d<T> operator-(vector2d<T> v) const { return vector2d<T>(X-v.X, Y-v.Y); }
    vector2d<T> operator+(vector2d<T> v) const { return vector2d<T>(X+v.X, Y+v.Y); }
    void operator=(const vector2d<T> v) { X = v.X; Y = v.Y;}
    void operator*=(const T v) { X = X*v; Y = Y*v;}

    friend inline std::ostream& operator << (std::ostream &o,const vector2d<T> &v){
        o << "(" << v.X << ", " << v.Y << ")";
        return o;
    }
};

typedef vector2d<float> vector2df;
typedef vector2d<int> vector2di;

#endif