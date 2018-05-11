#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

template<typename T>
class vector3d;

template<typename T>
class vector2d{

private:
public:
    // variables of vector
    T X, Y;      
    
    /**
     * Creates and empty 3d vector of all 0;
    */
    vector2d();
    
    /**
     * Creates and initializes 3d vector
    */
    vector2d(T nx, T ny);

    /**
     * Copy constructor
    */
    vector2d(const vector2d<T>& other);

    /**
     * Convert 3d in 2d
     */

    vector2d(const vector3d<T>& other);

    /**
     * returns length of the vector
    */
    float length();

    /**
     * convert to abs the values of the vector
     */
    vector2d absolute();

    /**
     * normalize the vector
     */

    void normalize();

    /**
     * Map the Angles between [-pi, pi]
     */

    void mapToRangePI();

    /**
     * Sets X Value
    */

    void setX(T newX);

    /**
     * Sets Y Value
    */

    void setY(T newY);


    //Operators
    vector2d<T> operator+(vector2d<T> v) const;
    vector2d<T> operator-(vector2d<T> v) const;
    vector2d<T> operator*(const T v) const;
    vector2d<T> operator/(const T v) const;

    void operator=(const vector2d<T> v);
    void operator*=(const T v);
    bool operator==(const vector2d<T> v);

    friend inline std::ostream& operator << (std::ostream &o,const vector2d<T> &v){
        o << "(" << v.X << ", " << v.Y << ")";
        return o;
    }
};

typedef vector2d<float> vector2df;
typedef vector2d<int> vector2di;

#endif