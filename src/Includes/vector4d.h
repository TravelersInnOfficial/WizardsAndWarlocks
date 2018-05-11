#ifndef VECTOR4d_H
#define VECTOR4d_H

#include <iostream>

template<typename T>
class vector4d{

private:
public:
    // variables of vector
    T X, Y, X2, Y2;      
    
    /**
     * Creates and empty 4d vector of all 0;
    */
    vector4d();
    
    /**
     * Creates and initializes 4d vector
    */
    vector4d(T nx, T ny, T nx2, T ny2);

    /**
     * Copy constructor
    */
    vector4d(const vector4d<T>& other);

    /**
     * returns length of the vector
    */
    float length();

    /**
     * Sets X Value
    */

    void setX(T newX);

    /**
     * Sets Y Value
    */

    void setY(T newY);

    /**
     * Sets X2 Value
    */

    void setX2(T newX2);

    /**
     * Sets Y2 Value
    */

    void setY2(T newY2);

    //Operators
    vector4d<T> operator+(vector4d<T> v) const;
    vector4d<T> operator-(vector4d<T> v) const;
    vector4d<T> operator*(const T v) const;
    vector4d<T> operator/(const T v) const;

    friend inline std::ostream& operator << (std::ostream &o,const vector4d<T> &v){
        o << "(" << v.X << ", " << v.Y << ", " << v.X2 << ", " << v.Y2 << ")";
        return o;
    }
};


typedef vector4d<float> vector4df;
typedef vector4d<int> vector4di;

#endif