#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>



template<typename T>
class vector3d{

private:
public:
    // variables of vector
    T X, Y, Z;      
    
    /**
     * Creates and empty 3d vector of all 0;
    */
    vector3d();
    
    /**
     * Creates and initializes 3d vector of same number
    */
    vector3d(T n);

    /**
     * Creates and initializes 3d vector
    */
    vector3d(T nx, T ny, T nz);

    /**
     * Copy constructor
    */
    vector3d(const vector3d<T>& other);

    /**
     * returns length of the vector
    */
    float length();

    /**
     * normalize the vector
     */

    void normalize();

    /**
     * convert the vector to radian
     */
    void toRadians();

    /**
     * convert the vector de degrees
     */
    void toDegrees();

    /**
     * Sets X Value
    */

    void setX(T newX);

    /**
     * Sets Y Value
    */

    void setY(T newY);

    /**
     * Sets Z Value
    */

    void setZ(T newZ);

    //Operators
    vector3d<T> operator+(const T v) const;
    vector3d<T> operator+(vector3d<T> v) const;
    vector3d<T> operator-(const T v) const;
    vector3d<T> operator-(vector3d<T> v) const;
    vector3d<T> operator*(const T v) const;
    vector3d<T> operator*(vector3d<T> v) const;
    vector3d<T> operator/(const T v) const;
    vector3d<T> operator/(vector3d<T> v) const;

    void operator=(const vector3d<T> v);
    void operator*=(const T v);

    bool operator==(const vector3d<T> v);


    friend inline std::ostream& operator <<(std::ostream &o,const vector3d<T> &v){
        o << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
        return o;
    }
};

    typedef vector3d<float> vector3df;
    typedef vector3d<int> vector3di;

#endif